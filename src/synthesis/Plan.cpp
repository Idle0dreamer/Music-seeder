#include "mq/synthesis/Plan.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <optional>
#include <set>
#include <span>
#include <sstream>
#include <utility>

namespace mq::synthesis {
namespace {

std::uint64_t mix(
    std::uint64_t seed,
    std::size_t ordinal) noexcept {
    auto value = seed + 0x9e3779b97f4a7c15ULL +
                 static_cast<std::uint64_t>(ordinal);
    value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31);
}

bool matches_prefix(
    const ::mq::kernel::Identity& candidate,
    std::span<const std::string> prefixes) {
    const auto value = candidate.str();
    return std::ranges::any_of(
        prefixes,
        [&](const auto& prefix) {
            return value.find(prefix) != std::string::npos;
        });
}

bool matches_fact_suffix(
    std::string_view value,
    std::string_view fact) {
    if (value == fact) {
        return true;
    }
    const auto suffix = "." + std::string(fact);
    return value.size() > suffix.size() && value.ends_with(suffix);
}

using Facts = std::set<std::string>;

void add_identity_fact(Facts& facts, const ::mq::kernel::Identity& identity) {
    if (!identity.name.empty()) {
        facts.insert(identity.name);
    }
}

Facts state_facts(const ::mq::kernel::state::Snapshot& state) {
    Facts facts;
    for (const auto& center : state.center.stack) {
        add_identity_fact(facts, center.identity);
    }
    if (state.jins.active) {
        add_identity_fact(facts, state.jins.active->identity);
    }
    for (const auto& path : state.path.completed) {
        add_identity_fact(facts, path.identity);
    }
    for (const auto& completion : state.sayr.history) {
        add_identity_fact(facts, completion.obligation.identity);
    }
    for (const auto& span : state.phrase.completed) {
        add_identity_fact(facts, span.identity);
        add_identity_fact(facts, span.function.identity);
        for (const auto& cadence : span.cadences) {
            add_identity_fact(facts, cadence.family);
            add_identity_fact(facts, cadence.event);
        }
    }
    for (const auto& [cell, count] : state.cell.occurrences) {
        if (count > 0) add_identity_fact(facts, cell.identity);
    }
    for (const auto& [motif, occurrences] : state.motif.occurrences) {
        if (occurrences.empty()) continue;
        add_identity_fact(facts, motif.identity);
        for (const auto& occurrence : occurrences) {
            if (occurrence.formula) {
                add_identity_fact(facts, occurrence.formula->identity);
            }
            if (occurrence.variation) {
                add_identity_fact(facts, occurrence.variation->identity);
            }
            if (occurrence.transformation) {
                add_identity_fact(facts, occurrence.transformation->identity);
            }
        }
    }
    return facts;
}

bool has_facts(
    const Facts& available,
    std::span<const std::string> required) {
    const auto matches = [&](const std::string& fact,
                             const std::string& requiredFact) {
        return matches_fact_suffix(fact, requiredFact);
    };
    return std::ranges::all_of(
        required,
        [&](const auto& requiredFact) {
            return std::ranges::any_of(
                available,
                [&](const auto& fact) {
                    return matches(fact, requiredFact);
                });
        });
}

std::expected<std::pair<::mq::kernel::performance::Plan, Facts>, std::string>
select_phrase_span(
    const ::mq::kernel::state::Snapshot& state,
    const ::mq::kernel::performance::Plan& source,
    std::string_view selector) {
    const auto span = std::ranges::find_if(
        state.phrase.completed,
        [&](const auto& value) {
            return matches_fact_suffix(value.identity.name, selector);
        });
    if (span == state.phrase.completed.end()) {
        return std::unexpected(
            "route derivation has no completed phrase span " +
            std::string(selector));
    }
    const auto first = std::ranges::find(
        state.melody.history,
        span->first,
        &::mq::kernel::performance::Event::identity);
    const auto last = std::ranges::find(
        state.melody.history,
        span->last,
        &::mq::kernel::performance::Event::identity);
    if (first == state.melody.history.end() ||
        last == state.melody.history.end() || first > last) {
        return std::unexpected(
            "phrase span does not map to the generated event history: " +
            std::string(selector));
    }
    const auto firstIndex = static_cast<std::size_t>(
        std::distance(state.melody.history.begin(), first));
    const auto lastIndex = static_cast<std::size_t>(
        std::distance(state.melody.history.begin(), last));
    if (lastIndex >= source.events.size()) {
        return std::unexpected(
            "phrase span does not map to the generated performance plan: " +
            std::string(selector));
    }

    const auto offset = source.events[firstIndex].onset;
    const auto finalEvent = source.events[lastIndex];
    const auto eventEnd = finalEvent.onset + finalEvent.duration;
    ::mq::kernel::performance::Plan selected;
    selected.events.reserve(lastIndex - firstIndex + 1);
    for (std::size_t index = firstIndex; index <= lastIndex; ++index) {
        auto event = source.events[index];
        event.onset -= offset;
        selected.events.push_back(std::move(event));
    }
    for (const auto& pause : source.pauses) {
        if (pause.onset < offset || pause.onset > eventEnd) {
            continue;
        }
        auto copied = pause;
        copied.onset -= offset;
        selected.pauses.push_back(std::move(copied));
    }
    if (!selected.well_formed()) {
        return std::unexpected(
            "selected phrase span is not a contiguous performance plan: " +
            std::string(selector));
    }

    Facts facts;
    add_identity_fact(facts, span->identity);
    add_identity_fact(facts, span->function.identity);
    for (const auto& cadence : span->cadences) {
        add_identity_fact(facts, cadence.family);
        add_identity_fact(facts, cadence.event);
    }
    for (const auto& event : selected.events) {
        add_identity_fact(facts, event.target.event.identity);
        if (event.target.cell) add_identity_fact(facts, event.target.cell->identity);
        if (event.target.formula) add_identity_fact(facts, event.target.formula->identity);
        if (event.target.variation) add_identity_fact(facts, event.target.variation->identity);
        if (event.target.motif) add_identity_fact(facts, event.target.motif->identity);
        if (event.target.transformation) {
            add_identity_fact(facts, event.target.transformation->identity);
        }
    }
    return std::pair{std::move(selected), std::move(facts)};
}

struct Phrase {
    GeneratedPlan generated;
    ::mq::kernel::state::Snapshot state;
    Facts facts;
};

struct PhraseChoice {
    const ::mq::kernel::generate::Outcome* outcome{};
    ::mq::kernel::performance::Plan plan;
    Facts facts;
};

using PerformanceStage =
    ::mq::kernel::maqam::family::PerformanceStageSpec;
using PerformancePath = std::vector<const PerformanceStage*>;

std::vector<PerformancePath> enumerate_performance_paths(
    const ::mq::kernel::maqam::family::PerformanceSpec& performance,
    std::size_t requested) {
    const auto find_stage = [&](std::string_view name)
        -> const PerformanceStage* {
        const auto found = std::ranges::find(
            performance.stages, name, &PerformanceStage::name);
        return found == performance.stages.end() ? nullptr : &*found;
    };
    const auto terminal = [&](const PerformanceStage* stage) {
        return std::ranges::find(
                   performance.terminals, stage->name) !=
               performance.terminals.end();
    };

    std::size_t extension{};
    for (const auto& stage : performance.stages) {
        extension += stage.maximum;
    }
    const auto maximum_depth = requested + extension;
    std::vector<PerformancePath> paths;
    PerformancePath path;
    std::function<void(const PerformanceStage*)> visit =
        [&](const PerformanceStage* current) {
            if (current == nullptr) return;
            const auto base_size = path.size();
            for (std::size_t count = current->minimum;; ++count) {
                path.insert(path.end(), count, current);
                const bool complete = path.size() >= requested &&
                                      terminal(current);
                if (complete) {
                    paths.push_back(path);
                } else if (path.size() < maximum_depth) {
                    for (const auto& next : current->next) {
                        visit(find_stage(next));
                    }
                }
                path.resize(base_size);
                if (count == current->maximum) break;
            }
        };

    visit(find_stage(performance.start));
    return paths;
}

std::string phrase_fingerprint(
    const ::mq::kernel::performance::Plan& plan,
    std::string_view selector) {
    std::ostringstream result;
    result << selector << '|';
    for (const auto& event : plan.events) {
        result << event.target.event.identity.str() << '|';
        if (event.target.cell) result << event.target.cell->str();
        result << '|';
        if (event.target.formula) result << event.target.formula->str();
        result << '|';
        if (event.target.variation) result << event.target.variation->str();
        result << '|';
        if (event.target.transformation) {
            result << event.target.transformation->str();
        }
        result << ';';
    }
    for (const auto& pause : plan.pauses) {
        result << "pause:" << pause.function.str() << ';';
    }
    return result.str();
}

std::expected<Phrase, std::string> make_phrase(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    std::span<const std::string> candidate_prefixes,
    const std::optional<std::string>& avoid,
    const std::optional<std::string>& phrase_span) {
    const auto catalog = ::mq::kernel::maqam::Catalog::declared();
    const auto scaffold = catalog.build_executable(maqam);
    if (!scaffold) {
        return std::unexpected(scaffold.error());
    }
    const ::mq::kernel::eval::Context context{
        .jins = {&scaffold->ajnas},
        .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr},
        .grammar = {},
    };
    const ::mq::kernel::generate::Engine engine(*scaffold->profile, context);
    std::optional<Phrase> selected_phrase;
    constexpr std::uint64_t retry_stride = 0x9e3779b97f4a7c15ULL;
    for (std::size_t attempt = 0; attempt < 32; ++attempt) {
        const auto generated = engine.run(
            seed + static_cast<std::uint64_t>(attempt) * retry_stride,
            scaffold->generation.choice,
            scaffold->generation.production,
            scaffold->generation.projection,
            scaffold->generation.schema,
            {},
            ::mq::kernel::generate::Limits{.timing = timing});
        if (!generated) {
            return std::unexpected(generated.error().message);
        }
        std::vector<PhraseChoice> compatible;
        for (const auto& outcome : generated->legal) {
            if (!matches_prefix(outcome.candidate, candidate_prefixes)) {
                continue;
            }
            auto selectedPlan = outcome.plan;
            auto facts = state_facts(outcome.state);
            if (phrase_span) {
                const auto span = select_phrase_span(
                    outcome.state, outcome.plan, *phrase_span);
                if (!span) {
                    continue;
                }
                selectedPlan = std::move(span->first);
                facts = std::move(span->second);
            }
            const auto fingerprint = phrase_fingerprint(
                selectedPlan,
                phrase_span ? *phrase_span : "full");
            if (avoid && fingerprint == *avoid) {
                continue;
            }
            compatible.push_back({
                &outcome,
                std::move(selectedPlan),
                std::move(facts),
            });
        }
        if (compatible.empty()) {
            continue;
        }
        auto selected = std::move(
            compatible[mix(seed, attempt) % compatible.size()]);
        selected_phrase = Phrase{
            GeneratedPlan{
                selected.outcome->candidate,
                std::move(selected.plan),
                {selected.outcome->candidate},
                {},
                {phrase_span ? *phrase_span : "full"},
            },
            selected.outcome->state,
            std::move(selected.facts),
        };
        break;
    }
    if (!selected_phrase) {
        return std::unexpected(
            "could not select a non-repeating legal phrase for " +
            std::string(maqam));
    }

    // Every outcome here is already a complete, legal candidate. The active
    // collection performance state filters the legal set; the seed only
    // selects within that set.
    return std::move(*selected_phrase);
}

} // namespace

void append_plan(
    ::mq::kernel::performance::Plan& destination,
    const ::mq::kernel::performance::Plan& continuation) {
    const auto offset = destination.end();
    for (const auto& event : continuation.events) {
        auto copied = event;
        copied.onset += offset;
        destination.events.push_back(std::move(copied));
    }
    for (const auto& pause : continuation.pauses) {
        auto copied = pause;
        copied.onset += offset;
        destination.pauses.push_back(std::move(copied));
    }
}

std::expected<GeneratedPlan, std::string> make_plan(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    std::size_t repetitions) {
    if (repetitions == 0) {
        return std::unexpected("at least one performance phrase is required");
    }
    const auto catalog = ::mq::kernel::maqam::Catalog::declared();
    const auto* entry = catalog.find(maqam);
    if (entry == nullptr || !entry->specification ||
        !entry->specification->performance) {
        return std::unexpected(
            "maqam has no collection-owned performance graph: " +
            std::string(maqam));
    }
    const auto& performance = *entry->specification->performance;
    const auto paths = enumerate_performance_paths(performance, repetitions);
    if (paths.empty()) {
        return std::unexpected(
            "collection performance has no complete legal discourse path for " +
            std::string(maqam));
    }

    // A seed selects a complete collection path. It never selects a stage
    // transition while an incomplete performance is being assembled.
    const auto first_path = static_cast<std::size_t>(
        mix(seed, 0) % paths.size());
    std::string last_error = "no path was realized";
    for (std::size_t path_offset{}; path_offset < paths.size(); ++path_offset) {
        const auto& path = paths[(first_path + path_offset) % paths.size()];
        std::optional<GeneratedPlan> result;
        std::optional<std::string> previous;
        Facts memory;
        bool valid = true;
        for (std::size_t emitted{}; emitted < path.size(); ++emitted) {
            const auto* stage = path[emitted];
            if (!has_facts(memory, stage->required_facts)) {
                last_error =
                    "performance stage entry facts are not satisfied for " +
                    stage->name + " in " + std::string(maqam);
                valid = false;
                break;
            }
            const auto phrase = make_phrase(
                maqam,
                seed + static_cast<std::uint64_t>(emitted) *
                           0x9e3779b97f4a7c15ULL,
                timing,
                stage->candidate_prefixes,
                previous,
                stage->phrase_span);
            if (!phrase) {
                last_error = phrase.error();
                valid = false;
                break;
            }
            if (!has_facts(phrase->facts, stage->provided_facts)) {
                last_error =
                    "performance stage postconditions are not satisfied for " +
                    stage->name + " in " + std::string(maqam);
                valid = false;
                break;
            }
            if (!result) {
                result = GeneratedPlan{
                    phrase->generated.candidate,
                    phrase->generated.plan,
                    {phrase->generated.candidate},
                    {stage->name},
                    {stage->phrase_span ? *stage->phrase_span : "full"},
                };
            } else {
                result->phrase_candidates.push_back(phrase->generated.candidate);
                result->phrase_stages.push_back(stage->name);
                result->phrase_spans.push_back(
                    stage->phrase_span ? *stage->phrase_span : "full");
                append_plan(result->plan, phrase->generated.plan);
            }
            memory.insert(phrase->facts.begin(), phrase->facts.end());
            memory.insert(
                stage->provided_facts.begin(), stage->provided_facts.end());
            previous = phrase_fingerprint(
                phrase->generated.plan,
                stage->phrase_span ? *stage->phrase_span : "full");
        }
        if (valid && result) {
            return std::move(*result);
        }
    }
    return std::unexpected(
        "collection performance paths could not be realized for " +
        std::string(maqam) + ": " + last_error);
}

std::expected<GeneratedPlan, std::string> make_plan_for_duration(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    double duration_seconds) {
    if (!(duration_seconds > 0.0)) {
        return std::unexpected("performance duration must be positive");
    }
    std::size_t repetitions = 1;
    constexpr std::size_t refinement_limit = 16;
    for (std::size_t refinement{}; refinement < refinement_limit; ++refinement) {
        const auto generated = make_plan(
            maqam, seed, timing, repetitions);
        if (!generated) {
            return std::unexpected(generated.error());
        }
        const auto seconds = timing.seconds_per_unit.decimal() *
                             generated->plan.end().decimal();
        if (!(seconds > 0.0)) {
            return std::unexpected(
                "performance timing produced an empty continuous plan");
        }
        if (seconds >= duration_seconds) {
            return std::move(*generated);
        }
        const auto estimate = static_cast<std::size_t>(std::ceil(
            static_cast<double>(repetitions) * duration_seconds / seconds));
        repetitions = std::max(repetitions + 1, estimate);
    }
    return std::unexpected(
        "could not reach the requested performance duration within the "
        "bounded generation refinement");
}

std::string describe_plan(
    const ::mq::kernel::performance::Plan& plan) {
    std::ostringstream output;
    output << "structure:\n";
    for (std::size_t index = 0; index < plan.events.size(); ++index) {
        const auto& event = plan.events[index];
        const auto identity = [](const auto& value) {
            return value ? value->identity.str() : std::string("none");
        };
        output << "  event[" << index << "]"
               << " onset=" << event.onset.str()
               << " duration=" << event.duration.str()
               << " structural=" << event.target.event.identity.str()
               << " cell=" << identity(event.target.cell)
               << " formula=" << identity(event.target.formula)
               << " variation=" << identity(event.target.variation)
               << " motif=" << identity(event.target.motif)
               << " transformation=" << identity(event.target.transformation)
               << " transformation_provenance="
               << (event.target.transformation_provenance.empty()
                       ? "none"
                       : event.target.transformation_provenance)
               << " direction="
               << ::mq::kernel::motion::name(event.target.event.direction)
               << " articulation="
               << static_cast<int>(event.articulation)
               << " release="
               << (event.release ? event.release->duration.str() : "none")
               << " ornament="
               << (event.ornament ? event.ornament->family.str() : "none")
               << '\n';
    }
    for (std::size_t index = 0; index < plan.pauses.size(); ++index) {
        const auto& pause = plan.pauses[index];
        output << "  pause[" << index << "]"
               << " onset=" << pause.onset.str()
               << " duration=" << pause.duration.str()
               << " function=" << pause.function.str()
               << " provenance=" << pause.provenance << '\n';
    }
    return output.str();
}

} // namespace mq::synthesis
