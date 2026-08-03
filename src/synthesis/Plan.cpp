#include "mq/synthesis/Plan.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>
#include <optional>
#include <span>
#include <sstream>

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

std::expected<GeneratedPlan, std::string> make_phrase(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    std::span<const std::string> candidate_prefixes,
    const std::optional<::mq::kernel::Identity>& avoid) {
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
    std::optional<GeneratedPlan> selected_plan;
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
        std::vector<const ::mq::kernel::generate::Outcome*> compatible;
        for (const auto& outcome : generated->legal) {
            if (matches_prefix(outcome.candidate, candidate_prefixes) &&
                (!avoid || generated->legal.size() == 1 ||
                 outcome.candidate != *avoid)) {
                compatible.push_back(&outcome);
            }
        }
        if (compatible.empty()) {
            continue;
        }
        const auto* selected = compatible[mix(seed, attempt) % compatible.size()];
        selected_plan = GeneratedPlan{
            selected->candidate,
            selected->plan,
            {selected->candidate},
        };
        break;
    }
    if (!selected_plan) {
        return std::unexpected(
            "could not select a non-repeating legal phrase for " +
            std::string(maqam));
    }

    // Every outcome here is already a complete, legal candidate. The active
    // collection performance state filters the legal set; the seed only
    // selects within that set.
    return std::move(*selected_plan);
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
    const auto find_stage = [&](std::string_view name)
        -> const ::mq::kernel::maqam::family::PerformanceStageSpec* {
        const auto found = std::ranges::find(
            performance.stages,
            name,
            &::mq::kernel::maqam::family::PerformanceStageSpec::name);
        return found == performance.stages.end() ? nullptr : &*found;
    };
    const auto* current = find_stage(performance.start);
    if (current == nullptr) {
        return std::unexpected(
            "performance graph start stage disappeared for " +
            std::string(maqam));
    }
    std::size_t emitted{};
    std::size_t transitionOrdinal{};
    std::optional<GeneratedPlan> result;
    std::optional<::mq::kernel::Identity> previous;
    const auto terminal = [&](const auto* stage) {
        return std::ranges::find(
                   performance.terminals,
                   stage->name) != performance.terminals.end();
    };
    while (emitted < repetitions || !terminal(current)) {
        const auto requested = repetitions > emitted
                                   ? repetitions - emitted
                                   : std::size_t{1};
        const auto count = std::min(
            current->minimum +
                static_cast<std::size_t>(mix(seed, transitionOrdinal) %
                                          (current->maximum -
                                           current->minimum + 1)),
            requested);
        for (std::size_t occurrence{}; occurrence < count; ++occurrence) {
            const auto phrase = make_phrase(
                maqam,
                seed + static_cast<std::uint64_t>(emitted) *
                           0x9e3779b97f4a7c15ULL,
                timing,
                current->candidate_prefixes,
                previous);
            if (!phrase) {
                return std::unexpected(phrase.error());
            }
            if (!result) {
                result = GeneratedPlan{
                    phrase->candidate,
                    phrase->plan,
                    {phrase->candidate},
                };
            } else {
                result->phrase_candidates.push_back(phrase->candidate);
                append_plan(result->plan, phrase->plan);
            }
            previous = phrase->candidate;
            ++emitted;
        }
        ++transitionOrdinal;
        if (emitted >= repetitions && terminal(current)) {
            break;
        }
        if (current->next.empty()) {
            return std::unexpected(
                "performance graph reached a terminal before the requested "
                "duration for " + std::string(maqam));
        }
        const auto nextIndex = mix(seed, transitionOrdinal) %
                               current->next.size();
        current = find_stage(current->next[nextIndex]);
        if (current == nullptr) {
            return std::unexpected(
                "performance graph transition disappeared for " +
                std::string(maqam));
        }
    }
    if (!result) {
        return std::unexpected(
            "performance graph emitted no complete phrase for " +
            std::string(maqam));
    }
    return std::move(*result);
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
