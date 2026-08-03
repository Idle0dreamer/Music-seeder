#include "Internal.hpp"

#include "mq/kernel/generate/Stage.hpp"
#include "mq/kernel/operation/Operation.hpp"

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <functional>
#include <map>
#include <string_view>

namespace mq::kernel::maqam::family::detail {
namespace {

using Stage = ::mq::kernel::generate::Stage;

Identity occurrence(const Key& key, const Identity& candidate, std::string suffix) {
    return id(key, "occurrence." + candidate.name + "." + std::move(suffix));
}

Identity event(const Key& key, const Identity& candidate, std::string suffix) {
    return id(key, "event." + candidate.name + "." + std::move(suffix));
}

Identity stage(const Key& key, const Identity& candidate, std::string suffix) {
    return id(key, "stage." + candidate.name + "." + std::move(suffix));
}

Identity phrase(const Key& key, const Identity& candidate, std::string suffix) {
    return id(key, "phrase." + candidate.name + "." + std::move(suffix));
}

Stage makeStage(
    const Key& key,
    const Identity& candidate,
    std::string suffix,
    std::vector<operation::Any> actions) {
    return {stage(key, candidate, std::move(suffix)), std::move(actions)};
}

grammar::Term actions(
    const Key& key,
    const Stage& value,
    const std::string& prefix) {
    auto result = grammar::Term::atom(id(key, prefix + ".atom.0"), value.actions.front());
    for (std::size_t index = 1; index < value.actions.size(); ++index) {
        result = grammar::Term::seq(
            id(key, prefix + ".seq." + std::to_string(index)),
            std::move(result),
            grammar::Term::atom(
                id(key, prefix + ".atom." + std::to_string(index)),
                value.actions[index]));
    }
    return result;
}

grammar::Term candidateTerm(
    const Key& key,
    const std::vector<Stage>& stages,
    const Identity& candidate) {
    auto body = grammar::Term::stage(
        id(key, "term." + candidate.name + ".stage.0"),
        stages.front().identity,
        actions(key, stages.front(), "term." + candidate.name + ".stage.0"));
    for (std::size_t index = 1; index < stages.size(); ++index) {
        const auto prefix =
            "term." + candidate.name + ".stage." + std::to_string(index);
        body = grammar::Term::seq(
            id(key, prefix + ".seq"),
            std::move(body),
            grammar::Term::stage(
                id(key, prefix),
                stages[index].identity,
                actions(key, stages[index], prefix)));
    }
    return grammar::Term::candidate(
        id(key, "term." + candidate.name + ".candidate"),
        candidate,
        std::move(body));
}

struct ActionContext {
    const Key& key;
    const Identity& candidate;
    const BranchKey* branch{};
    std::optional<Identity> previousDescent;
    std::size_t variant{};
};

std::expected<Rational, std::string> rational(std::string_view value) {
    const auto separator = value.find('/');
    const auto numeratorText = value.substr(
        0,
        separator == std::string_view::npos ? value.size() : separator);
    const auto denominatorText = separator == std::string_view::npos
                                     ? std::string_view{"1"}
                                     : value.substr(separator + 1);
    std::int64_t numerator{};
    std::int64_t denominator{};
    const auto parsedNumerator = std::from_chars(
        numeratorText.data(),
        numeratorText.data() + numeratorText.size(),
        numerator);
    const auto parsedDenominator = std::from_chars(
        denominatorText.data(),
        denominatorText.data() + denominatorText.size(),
        denominator);
    if (parsedNumerator.ec != std::errc{} ||
        parsedNumerator.ptr != numeratorText.data() + numeratorText.size() ||
        parsedDenominator.ec != std::errc{} ||
        parsedDenominator.ptr !=
            denominatorText.data() + denominatorText.size() ||
        denominator <= 0) {
        return std::unexpected("invalid rational action argument: " +
                               std::string(value));
    }
    return Rational(numerator, denominator);
}

std::expected<void, std::string> arity(
    const ActionSpec& action,
    std::size_t minimum,
    std::size_t maximum) {
    if (action.arguments.size() < minimum ||
        action.arguments.size() > maximum) {
        return std::unexpected(
            "invalid argument count for collection operation " +
            action.operation);
    }
    return {};
}

std::expected<Identity, std::string> reference(
    const ActionContext& context,
    std::string_view token) {
    const std::map<std::string, Identity> fixed{
        {"center.root", context.key.centerRoot},
        {"center.upper", context.key.centerUpper},
        {"jins.root", context.key.jinsRoot},
        {"role.tonic", context.key.roleTonic},
        {"role.ghammaz", context.key.roleGhammaz},
        {"role.upper", context.key.roleUpper},
        {"role.extension", context.key.roleExtension},
        {"region.root", context.key.regionRoot},
        {"region.upper", context.key.regionUpper},
        {"gesture.establish", context.key.gestureEstablish},
        {"gesture.ascent", context.key.gestureAscent},
        {"gesture.descent", context.key.gestureDescent},
        {"gesture.resolution", context.key.gestureResolution},
        {"phrase.question", context.key.phraseQuestion},
        {"phrase.response", context.key.phraseResponse},
        {"cell.establish", context.key.cellEstablish},
        {"cell.develop", context.key.cellDevelop},
        {"cell.climax", context.key.cellClimax},
        {"cell.return", context.key.cellReturn},
        {"formula.establish", context.key.formulaEstablish},
        {"formula.develop", context.key.formulaDevelop},
        {"formula.develop-variation", context.key.formulaDevelopVariation},
        {"formula.climax", context.key.formulaClimax},
        {"formula.return", context.key.formulaReturn},
        {"cadence.local", context.key.cadenceLocal},
        {"cadence.return", context.key.cadenceReturn},
        {"baggage.extension", context.key.baggageExtension},
        {"obligation.establish", id(context.key, "obligation.establish")},
        {"obligation.settle", id(context.key, "obligation.settle")},
        {"obligation.expand", id(context.key, "obligation.expand")},
        {"obligation.climax", id(context.key, "obligation.climax")},
    };
    if (const auto found = fixed.find(std::string(token));
        found != fixed.end()) {
        return found->second;
    }
    if (token == "previous-descent" || token == "current-descent") {
        if (!context.previousDescent) {
            return std::unexpected(
                "collection operation requires an active descent occurrence");
        }
        return *context.previousDescent;
    }
    if (token == "branch.jins" || token == "branch.center" ||
        token == "branch.tonic" || token == "branch.ghammaz" ||
        token == "branch.descent" || token == "branch.path" ||
        token == "branch.travel" || token == "branch.restore") {
        if (context.branch == nullptr) {
            return std::unexpected(
                "collection operation requires a branch reference");
        }
        const std::map<std::string, Identity> branch{
            {"branch.jins", context.branch->jins},
            {"branch.center", context.branch->center},
            {"branch.tonic", context.branch->tonic},
            {"branch.ghammaz", context.branch->ghammaz},
            {"branch.descent", context.branch->descent},
            {"branch.path", context.branch->path},
            {"branch.travel", context.branch->travel},
            {"branch.restore", context.branch->restore},
        };
        return branch.at(std::string(token));
    }
    if (token == "event.branch") {
        if (context.branch == nullptr) {
            return std::unexpected(
                "event.branch requires a branch reference");
        }
        return event(
            context.key,
            context.candidate,
            "branch." + context.branch->jins.name);
    }
    if (token.starts_with("event.")) {
        return event(
            context.key,
            context.candidate,
            std::string(token.substr(6)));
    }
    if (token == "occurrence.branch-descent") {
        if (context.branch == nullptr) {
            return std::unexpected(
                "occurrence.branch-descent requires a branch reference");
        }
        return occurrence(
            context.key,
            context.candidate,
            "sequence-descent." + context.branch->jins.name);
    }
    if (token.starts_with("occurrence.")) {
        return occurrence(
            context.key,
            context.candidate,
            std::string(token.substr(11)));
    }
    if (token == "phrase.establish" || token == "phrase.return") {
        return phrase(
            context.key,
            context.candidate,
            std::string(token.substr(7)));
    }
    return std::unexpected(
        "unknown collection reference: " + std::string(token));
}

std::expected<motion::Direction, std::string> direction(
    const ActionContext& context,
    std::string_view token) {
    if (token == "branch.direction") {
        if (context.branch == nullptr) {
            return std::unexpected(
                "branch.direction requires a branch reference");
        }
        return context.branch->direction;
    }
    const std::map<std::string, motion::Direction> values{
        {"start", motion::Direction::Start},
        {"same", motion::Direction::Same},
        {"rise", motion::Direction::Rise},
        {"fall", motion::Direction::Fall},
    };
    const auto found = values.find(std::string(token));
    if (found == values.end()) {
        return std::unexpected(
            "unknown collection direction: " + std::string(token));
    }
    return found->second;
}

std::expected<tonicization::Level, std::string> level(
    std::string_view token) {
    const std::map<std::string, tonicization::Level> values{
        {"color", tonicization::Level::Color},
        {"internal", tonicization::Level::Internal},
        {"maqam", tonicization::Level::Maqam},
    };
    const auto found = values.find(std::string(token));
    if (found == values.end()) {
        return std::unexpected(
            "unknown collection tonicization level: " + std::string(token));
    }
    return found->second;
}

std::expected<phrase::Boundary, std::string> boundary(
    std::string_view token) {
    if (token == "open") {
        return phrase::Boundary::Open;
    }
    if (token == "closed") {
        return phrase::Boundary::Closed;
    }
    return std::unexpected(
        "unknown collection phrase boundary: " + std::string(token));
}

std::expected<Identity, std::string> function(
    const Key& key,
    std::string_view token) {
    if (token == "question") {
        return key.phraseQuestion;
    }
    if (token == "response") {
        return key.phraseResponse;
    }
    return std::unexpected(
        "unknown collection phrase function: " + std::string(token));
}

using ActionBuilder = std::function<std::expected<operation::Any, std::string>(
    const ActionContext&,
    const ActionSpec&)>;

std::expected<operation::Any, std::string> makeAction(
    const ActionContext& context,
    const ActionSpec& action) {
    const ActionBuilder emit =
        [](const ActionContext& c,
           const ActionSpec& a) -> std::expected<operation::Any, std::string> {
        if (const auto checked = arity(a, 2, 3); !checked) {
            return std::unexpected(checked.error());
        }
        const auto cellValue = reference(c, a.arguments[0]);
        const auto formulaValue = reference(c, a.arguments[1]);
        if (!cellValue) {
            return std::unexpected(cellValue.error());
        }
        if (!formulaValue) {
            return std::unexpected(formulaValue.error());
        }
        std::optional<sort::FormulaId> variation;
        if (a.arguments.size() == 3 &&
            (c.variant > 0 || a.operation == "emit.variation")) {
            const auto variationValue = reference(c, a.arguments[2]);
            if (!variationValue) {
                return std::unexpected(variationValue.error());
            }
            variation = sort::FormulaId{*variationValue};
        }
        return operation::Any{operation::Emit{
            sort::CellId{*cellValue},
            sort::FormulaId{*formulaValue},
            variation}};
    };
    const std::map<std::string, ActionBuilder> builders{
        {"anchor", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 1, 1); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto value = reference(c, a.arguments[0]);
             if (!value) {
                 return std::unexpected(value.error());
             }
             return operation::Any{operation::Anchor{sort::CenterId{*value}}};
         }},
        {"enter", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 1, 1); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto value = reference(c, a.arguments[0]);
             if (!value) {
                 return std::unexpected(value.error());
             }
             return operation::Any{operation::Enter{sort::JinsId{*value}}};
         }},
        {"begin", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 2, 2); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto p = reference(c, a.arguments[0]);
             const auto f = function(c.key, a.arguments[1]);
             if (!p) {
                 return std::unexpected(p.error());
             }
             if (!f) {
                 return std::unexpected(f.error());
             }
             return operation::Any{operation::Begin{
                 sort::PhraseId{*p},
                 phrase::Function{*f}}};
         }},
        {"end", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 2, 2); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto p = reference(c, a.arguments[0]);
             const auto b = boundary(a.arguments[1]);
             if (!p) {
                 return std::unexpected(p.error());
             }
             if (!b) {
                 return std::unexpected(b.error());
             }
             return operation::Any{operation::End{
                 sort::PhraseId{*p},
                 *b}};
         }},
        {"gesture.begin", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 2, 2); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto occurrenceValue = reference(c, a.arguments[0]);
             const auto familyValue = reference(c, a.arguments[1]);
             if (!occurrenceValue) {
                 return std::unexpected(occurrenceValue.error());
             }
             if (!familyValue) {
                 return std::unexpected(familyValue.error());
             }
             return operation::Any{operation::gesture::Begin{
                 *occurrenceValue,
                 *familyValue}};
         }},
        {"gesture.end", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 1, 1); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto value = reference(c, a.arguments[0]);
             if (!value) {
                 return std::unexpected(value.error());
             }
             return operation::Any{operation::gesture::End{*value}};
         }},
        {"place", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 5, 5); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto e = reference(c, a.arguments[0]);
             const auto r = reference(c, a.arguments[1]);
             const auto d = direction(c, a.arguments[2]);
             const auto regionValue = reference(c, a.arguments[3]);
             if (!e) {
                 return std::unexpected(e.error());
             }
             if (!r) {
                 return std::unexpected(r.error());
             }
             if (!d) {
                 return std::unexpected(d.error());
             }
             if (!regionValue) {
                 return std::unexpected(regionValue.error());
             }
             std::optional<sort::BaggageId> baggageValue;
             if (a.arguments[4] != "-") {
                 const auto baggage = reference(c, a.arguments[4]);
                 if (!baggage) {
                     return std::unexpected(baggage.error());
                 }
                 baggageValue = sort::BaggageId{*baggage};
             }
             return operation::Any{operation::Place{
                 sort::EventId{*e},
                 sort::RoleId{*r},
                 *d,
                 sort::RegionId{*regionValue},
                 baggageValue}};
         }},
        {"emit", emit},
        {"emit.variation", emit},
        {"emphasize", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 2, 2); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto roleValue = reference(c, a.arguments[0]);
             const auto amount = rational(a.arguments[1]);
             if (!roleValue) {
                 return std::unexpected(roleValue.error());
             }
             if (!amount) {
                 return std::unexpected(amount.error());
             }
             return operation::Any{operation::Emphasize{
                 sort::RoleId{*roleValue}, *amount}};
         }},
        {"dwell", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 2, 2); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto roleValue = reference(c, a.arguments[0]);
             const auto amount = rational(a.arguments[1]);
             if (!roleValue) {
                 return std::unexpected(roleValue.error());
             }
             if (!amount) {
                 return std::unexpected(amount.error());
             }
             return operation::Any{operation::Dwell{
                 sort::RoleId{*roleValue}, *amount}};
         }},
        {"cadence", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 3, 3); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto familyValue = reference(c, a.arguments[0]);
             const auto evidence = rational(a.arguments[1]);
             const auto strength = rational(a.arguments[2]);
             if (!familyValue) {
                 return std::unexpected(familyValue.error());
             }
             if (!evidence) {
                 return std::unexpected(evidence.error());
             }
             if (!strength) {
                 return std::unexpected(strength.error());
             }
             return operation::Any{operation::Cadence{
                 sort::FamilyId{*familyValue}, *evidence, *strength}};
         }},
        {"tonicize", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 2, 2); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto jinsValue = reference(c, a.arguments[0]);
             const auto levelValue = level(a.arguments[1]);
             if (!jinsValue) {
                 return std::unexpected(jinsValue.error());
             }
             if (!levelValue) {
                 return std::unexpected(levelValue.error());
             }
             return operation::Any{operation::Tonicize{
                 sort::JinsId{*jinsValue}, *levelValue}};
         }},
        {"modulate", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 3, 3); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto pathValue = reference(c, a.arguments[0]);
             const auto centerValue = reference(c, a.arguments[1]);
             const auto levelValue = level(a.arguments[2]);
             if (!pathValue) {
                 return std::unexpected(pathValue.error());
             }
             if (!centerValue) {
                 return std::unexpected(centerValue.error());
             }
             if (!levelValue) {
                 return std::unexpected(levelValue.error());
             }
             return operation::Any{operation::Modulate{
                 sort::PathId{*pathValue},
                 sort::CenterId{*centerValue},
                 *levelValue}};
         }},
        {"return", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 1, 1); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto centerValue = reference(c, a.arguments[0]);
             if (!centerValue) {
                 return std::unexpected(centerValue.error());
             }
             return operation::Any{operation::Return{
                 sort::CenterId{*centerValue}}};
         }},
        {"sayr.fulfill", [](const auto& c, const auto& a) -> std::expected<operation::Any, std::string> {
             if (const auto checked = arity(a, 1, 1); !checked) {
                 return std::unexpected(checked.error());
             }
             const auto obligationValue = reference(c, a.arguments[0]);
             if (!obligationValue) {
                 return std::unexpected(obligationValue.error());
             }
             return operation::Any{operation::sayr::Fulfill{
                 sort::ObligationId{*obligationValue}}};
         }},
    };
    const auto found = builders.find(action.operation);
    if (found == builders.end()) {
        return std::unexpected(
            "unknown collection operation: " + action.operation);
    }
    return found->second(context, action);
}

std::expected<std::vector<Stage>, std::string> routeSteps(
    const Key& key,
    const Identity& candidate,
    const RouteKey& route,
    std::size_t variant) {
    std::vector<Stage> stages;
    std::optional<Identity> previousDescent;
    for (const auto& step : route.steps) {
        const auto branch = [&]() -> const BranchKey* {
            if (!step.branch || *step.branch >= key.branches.size()) {
                return nullptr;
            }
            return &key.branches[*step.branch];
        }();
        ActionContext context{key, candidate, branch, previousDescent, variant};
        std::vector<operation::Any> actions;
        for (const auto& specification : step.actions) {
            if (specification.operation == "gesture.end.if-active") {
                if (specification.arguments.size() != 1 ||
                    specification.arguments.front() != "previous-descent") {
                    return std::unexpected(
                        step.name + ": invalid conditional gesture close");
                }
                if (context.previousDescent) {
                    actions.push_back(operation::gesture::End{
                        *context.previousDescent});
                }
                continue;
            }
            if (specification.operation == "sayr.fulfill.route") {
                if (specification.arguments.size() != 1 ||
                    specification.arguments.front() != "restore") {
                    return std::unexpected(
                        step.name + ": invalid route fulfillment action");
                }
                for (const auto index : route.branches) {
                    actions.push_back(operation::sayr::Fulfill{
                        sort::ObligationId{key.branches[index].restore}});
                }
                continue;
            }
            const auto action = makeAction(context, specification);
            if (!action) {
                return std::unexpected(
                    step.name + ": " + action.error());
            }
            actions.push_back(*action);
        }
        if (actions.empty()) {
            return std::unexpected(
                "collection step has no operations: " + step.name);
        }
        stages.push_back(makeStage(
            key,
            candidate,
            route.route.name + "." + step.name,
            std::move(actions)));
        const auto descended = std::ranges::find_if(
            step.actions,
            [](const auto& action) {
                return action.operation == "gesture.begin" &&
                       !action.arguments.empty() &&
                       action.arguments.front() == "occurrence.branch-descent";
            });
        if (descended != step.actions.end() && branch != nullptr) {
            previousDescent = occurrence(
                key,
                candidate,
                "sequence-descent." + branch->jins.name);
        }
    }
    return stages;
}

pitch::field::project::Plan projection(const Key& key) {
    namespace pp = pitch::field::project;
    return {{
        pp::role::Read{key.keyRole},
        pp::jins::Read{key.keyJins},
        pp::motion::Read{
            key.keyMotion,
            key.motionStart,
            key.motionSame,
            key.motionRise,
            key.motionFall,
        },
        pp::registral::Read{key.keyRegion},
        pp::baggage::Read{key.keyBaggage, pp::Presence::Optional},
        pp::gesture::Read{key.keyGesture, pp::Presence::Optional},
        pp::phrase::function::Read{key.keyFunction},
        pp::phrase::cadence::Read{key.keyCadence, pp::Presence::Optional},
        pp::phrase::boundary::Read{
            key.keyBoundary,
            key.boundaryOpen,
            key.boundaryClosed,
            pp::Presence::Optional,
        },
    }};
}

pitch::field::Schema schema(const Key& key) {
    namespace pf = pitch::field;
    const auto tier = id(key, "pitch.tier");
    const auto anchor = id(key, "pitch.rule.anchor");
    const auto ghammaz = id(key, "pitch.rule.ghammaz");
    const auto extension = id(key, "pitch.rule.extension");
    const auto returnTonic = id(key, "pitch.rule.return-tonic");
    pf::Schema result;
    result.variables = {
        key.roleTonic,
        key.roleGhammaz,
        key.roleUpper,
        key.roleExtension,
    };
    for (const auto& branch : key.branches) {
        if (std::ranges::find(result.variables, branch.tonic) ==
            result.variables.end()) {
            result.variables.push_back(branch.tonic);
        }
        if (std::ranges::find(result.variables, branch.ghammaz) ==
            result.variables.end()) {
            result.variables.push_back(branch.ghammaz);
        }
        if (std::ranges::find(result.variables, branch.descent) ==
            result.variables.end()) {
            result.variables.push_back(branch.descent);
        }
    }
    result.tiers = {tier};
    result.rules.push_back(pf::Rule{
        pf::Guard{{}},
        pitch::Equation{
            anchor,
            {{key.roleTonic, Rational(1)}},
            {},
            key.package + " tonic anchor; structural frequency authority",
        },
    });
    const auto addAim = [&](std::vector<pitch::field::Fact> facts,
                            Identity identity,
                            std::vector<pitch::Term> terms,
                            pitch::Expression target,
                            std::string provenance) {
        result.rules.push_back(pf::Rule{
            pf::Guard{std::move(facts)},
            pf::Aim{
                tier,
                {
                    std::move(identity),
                    std::move(terms),
                    std::move(target),
                    Rational(1),
                    std::move(provenance),
                },
            },
        });
    };
    addAim(
        {{key.keyRole, key.roleTonic},
         {key.keyMotion, key.motionStart},
         {key.keyRegion, key.regionRoot},
         {key.keyGesture, key.gestureAscent}},
        id(key, "pitch.rule.tonic"),
        {{key.roleTonic, Rational(1)}},
        {},
        key.source + ";establishment");
    addAim(
        {{key.keyRole, key.roleGhammaz},
         {key.keyMotion, key.motionRise},
         {key.keyRegion, key.regionUpper},
         {key.keyGesture, key.gestureAscent}},
        ghammaz,
        {{key.roleGhammaz, Rational(1)}, {key.roleTonic, Rational(-1)}},
        key.ghammaz,
        key.source + ";ghammaz");
    addAim(
        {{key.keyRole, key.roleExtension},
         {key.keyMotion, key.motionRise},
         {key.keyRegion, key.regionUpper},
         {key.keyBaggage, key.baggageExtension},
         {key.keyGesture, key.gestureAscent},
         {key.keyCadence, key.cadenceLocal},
         {key.keyBoundary, key.boundaryClosed}},
        extension,
        {{key.roleExtension, Rational(1)}, {key.roleTonic, Rational(-1)}},
        key.extension,
        key.source + ";upper-closure");
    for (const auto& branch : key.branches) {
        addAim(
        {{key.keyJins, branch.jins},
             {key.keyRole, branch.descent},
             {key.keyMotion, branch.motion},
             {key.keyRegion, key.regionUpper},
             {key.keyGesture, key.gestureDescent}},
            id(key, "pitch.rule.branch." + branch.jins.name),
            {{branch.descent, Rational(1)}, {key.roleTonic, Rational(-1)}},
            branch.target,
            branch.source + ";branch-target");
    }
    addAim(
        {{key.keyRole, key.roleTonic},
         {key.keyMotion, key.motionFall},
         {key.keyRegion, key.regionRoot},
         {key.keyGesture, key.gestureResolution},
         {key.keyCadence, key.cadenceReturn},
         {key.keyBoundary, key.boundaryClosed}},
        returnTonic,
        {{key.roleTonic, Rational(1)}},
        {},
        key.source + ";return");
    std::vector<Identity> needs{anchor, ghammaz, extension, returnTonic};
    for (const auto& branch : key.branches) {
        needs.push_back(id(key, "pitch.rule.branch." + branch.jins.name));
    }
    result.needs = {{id(key, "pitch.need"), std::move(needs)}};
    return result;
}

} // namespace

std::expected<Generation, std::string> generation(const Key& key) {
    if (key.routes.empty()) {
        return std::unexpected("family package declares no generation routes");
    }
    std::vector<grammar::Branch> branches;
    for (const auto& route : key.routes) {
        if (!route.valid) {
            return std::unexpected(
                "generation route references an unknown branch: " +
                route.route.str());
        }
        if (route.variants == 0) {
            return std::unexpected(
                "generation route declares zero variants: " +
                route.route.str());
        }
        for (std::size_t variant = 0; variant < route.variants; ++variant) {
            const auto suffix = route.variants == 1
                                     ? route.route.name
                                     : route.route.name + ".variant." +
                                           std::to_string(variant);
            const auto candidate = id(key, "candidate." + suffix);
            const auto stages = routeSteps(key, candidate, route, variant);
            if (!stages) {
                return std::unexpected(stages.error());
            }
            if (stages->empty()) {
                return std::unexpected(
                    "generation route produced no stages: " +
                    route.route.str());
            }
            branches.push_back({
                id(key, "branch." + suffix),
                {},
                candidateTerm(key, *stages, candidate),
            });
        }
    }
    auto production = grammar::Term::alt(id(key, "production"), std::move(branches));
    if (!production) {
        return std::unexpected(production.error());
    }
    return Generation{
        key.choice,
        std::move(*production),
        projection(key),
        schema(key),
    };
}

} // namespace mq::kernel::maqam::family::detail
