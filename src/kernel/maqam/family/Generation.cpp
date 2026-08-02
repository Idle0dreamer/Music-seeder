#include "Internal.hpp"

#include "mq/kernel/generate/Stage.hpp"
#include "mq/kernel/operation/Operation.hpp"

#include <algorithm>

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

Stage establishment(const Key& key, const Identity& candidate) {
    const auto p = phrase(key, candidate, "establish");
    const auto g = occurrence(key, candidate, "ascent");
    return makeStage(key, candidate, "establish", {
        operation::Anchor{sort::CenterId{key.centerRoot}},
        operation::Enter{sort::JinsId{key.jinsRoot}},
        operation::Begin{sort::PhraseId{p}, phrase::Function{key.phraseQuestion}},
        operation::gesture::Begin{g, key.gestureAscent},
        operation::Place{
            sort::EventId{event(key, candidate, "tonic")},
            sort::RoleId{key.roleTonic},
            motion::Direction::Start,
            sort::RegionId{key.regionRoot},
            std::nullopt,
        },
        operation::Emit{
            sort::CellId{key.cellEstablish},
            sort::FormulaId{key.formulaEstablish},
        },
    });
}

Stage development(const Key& key, const Identity& candidate) {
    return makeStage(key, candidate, "development", {
        operation::Place{
            sort::EventId{event(key, candidate, "ghammaz")},
            sort::RoleId{key.roleGhammaz},
            motion::Direction::Rise,
            sort::RegionId{key.regionUpper},
            std::nullopt,
        },
        operation::Emphasize{sort::RoleId{key.roleGhammaz}, Rational(3)},
        operation::Dwell{sort::RoleId{key.roleGhammaz}, Rational(2)},
        operation::Emit{
            sort::CellId{key.cellDevelop},
            sort::FormulaId{key.formulaDevelop},
        },
        operation::Cadence{
            sort::FamilyId{key.cadenceLocal},
            Rational(1),
            Rational(1, 2),
        },
    });
}

Stage climax(
    const Key& key,
    const Identity& candidate,
    const BranchKey& branch) {
    const auto p = phrase(key, candidate, "establish");
    return makeStage(key, candidate, "climax", {
        operation::Place{
            sort::EventId{event(key, candidate, "extension")},
            sort::RoleId{key.roleExtension},
            motion::Direction::Rise,
            sort::RegionId{key.regionUpper},
            sort::BaggageId{key.baggageExtension},
        },
        operation::Emit{
            sort::CellId{key.cellClimax},
            sort::FormulaId{key.formulaClimax},
        },
        operation::gesture::End{occurrence(key, candidate, "ascent")},
        operation::Cadence{
            sort::FamilyId{key.cadenceLocal},
            Rational(1),
            Rational(3, 4),
        },
        operation::End{sort::PhraseId{p}, phrase::Boundary::Closed},
        operation::sayr::Fulfill{sort::ObligationId{id(key, "obligation.establish")}},
        operation::sayr::Fulfill{sort::ObligationId{id(key, "obligation.expand")}},
        operation::sayr::Fulfill{sort::ObligationId{id(key, "obligation.climax")}},
        operation::Tonicize{
            sort::JinsId{branch.jins},
            tonicization::Level::Internal,
        },
        operation::Modulate{
            sort::PathId{branch.path},
            sort::CenterId{branch.center},
            tonicization::Level::Internal,
        },
        operation::sayr::Fulfill{sort::ObligationId{branch.travel}},
    });
}

Stage branchDescent(
    const Key& key,
    const Identity& candidate,
    const BranchKey& branch) {
    const auto p = phrase(key, candidate, "return");
    const auto descent = occurrence(key, candidate, "descent");
    return makeStage(key, candidate, "branch-descent", {
        operation::Begin{sort::PhraseId{p}, phrase::Function{key.phraseResponse}},
        operation::gesture::Begin{descent, key.gestureDescent},
        operation::Place{
            sort::EventId{event(key, candidate, "branch")},
            sort::RoleId{branch.ghammaz},
            branch.direction,
            sort::RegionId{key.regionUpper},
            std::nullopt,
        },
        operation::Emit{
            sort::CellId{key.cellDevelop},
            sort::FormulaId{key.formulaDevelopVariation},
        },
    });
}

Stage restore(
    const Key& key,
    const Identity& candidate,
    const BranchKey& branch) {
    const auto p = phrase(key, candidate, "return");
    const auto descent = occurrence(key, candidate, "descent");
    const auto resolution = occurrence(key, candidate, "resolution");
    return makeStage(key, candidate, "restore", {
        operation::gesture::End{descent},
        operation::Enter{sort::JinsId{key.jinsRoot}},
        operation::gesture::Begin{resolution, key.gestureResolution},
        operation::Place{
            sort::EventId{event(key, candidate, "tonic-return")},
            sort::RoleId{key.roleTonic},
            motion::Direction::Fall,
            sort::RegionId{key.regionRoot},
            std::nullopt,
        },
        operation::Emit{
            sort::CellId{key.cellReturn},
            sort::FormulaId{key.formulaReturn},
        },
        operation::gesture::End{resolution},
        operation::Cadence{
            sort::FamilyId{key.cadenceReturn},
            Rational(1),
            Rational(1),
        },
        operation::End{sort::PhraseId{p}, phrase::Boundary::Closed},
        operation::Return{sort::CenterId{key.centerRoot}},
        operation::sayr::Fulfill{sort::ObligationId{branch.restore}},
    });
}

Stage orderedContinuation(
    const Key& key,
    const Identity& candidate,
    const BranchKey& branch) {
    const auto p = phrase(key, candidate, "return");
    const auto descent = occurrence(
        key,
        candidate,
        "ordered-descent." + branch.jins.name);
    return makeStage(key, candidate, "ordered." + branch.jins.name, {
        operation::Begin{sort::PhraseId{p}, phrase::Function{key.phraseResponse}},
        operation::Enter{sort::JinsId{branch.jins}},
        operation::Tonicize{
            sort::JinsId{branch.jins},
            tonicization::Level::Internal,
        },
        operation::gesture::Begin{descent, key.gestureDescent},
        operation::Place{
            sort::EventId{event(key, candidate, "ordered." + branch.jins.name)},
            sort::RoleId{branch.ghammaz},
            branch.direction,
            sort::RegionId{key.regionUpper},
            std::nullopt,
        },
        operation::Modulate{
            sort::PathId{branch.path},
            sort::CenterId{branch.center},
            tonicization::Level::Internal,
        },
        operation::Emit{
            sort::CellId{key.cellDevelop},
            sort::FormulaId{key.formulaDevelopVariation},
        },
        operation::sayr::Fulfill{sort::ObligationId{branch.travel}},
    });
}

Stage orderedRestore(
    const Key& key,
    const Identity& candidate) {
    const auto p = phrase(key, candidate, "return");
    const auto descent = occurrence(
        key,
        candidate,
        "ordered-descent." + key.branches.back().jins.name);
    const auto resolution = occurrence(key, candidate, "ordered-resolution");
    std::vector<operation::Any> actions{
        operation::gesture::End{descent},
        operation::Enter{sort::JinsId{key.jinsRoot}},
        operation::gesture::Begin{resolution, key.gestureResolution},
        operation::Place{
            sort::EventId{event(key, candidate, "ordered-tonic-return")},
            sort::RoleId{key.roleTonic},
            motion::Direction::Fall,
            sort::RegionId{key.regionRoot},
            std::nullopt,
        },
        operation::Emit{
            sort::CellId{key.cellReturn},
            sort::FormulaId{key.formulaReturn},
        },
        operation::gesture::End{resolution},
        operation::Cadence{
            sort::FamilyId{key.cadenceReturn},
            Rational(1),
            Rational(1),
        },
        operation::End{sort::PhraseId{p}, phrase::Boundary::Closed},
        operation::Return{sort::CenterId{key.centerRoot}},
    };
    for (const auto& branch : key.branches) {
        actions.push_back(operation::sayr::Fulfill{
            sort::ObligationId{branch.restore}});
    }
    return makeStage(key, candidate, "ordered.restore", std::move(actions));
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

std::vector<Stage> journey(
    const Key& key,
    const Identity& candidate,
    const BranchKey& branch) {
    return {
        establishment(key, candidate),
        development(key, candidate),
        climax(key, candidate, branch),
        branchDescent(key, candidate, branch),
        restore(key, candidate, branch),
    };
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
             {key.keyRole, branch.ghammaz},
             {key.keyMotion, branch.motion},
             {key.keyRegion, key.regionUpper},
             {key.keyGesture, key.gestureDescent}},
            id(key, "pitch.rule.branch." + branch.jins.name),
            {{branch.ghammaz, Rational(1)}, {key.roleTonic, Rational(-1)}},
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
    if (key.ordered) {
        if (key.branches.size() != 2) {
            return std::unexpected(
                "ordered family package requires exactly two stations");
        }
        const auto candidate = id(key, "candidate.ordered");
        const auto stages = std::vector<Stage>{
            establishment(key, candidate),
            development(key, candidate),
            climax(key, candidate, key.branches.front()),
            orderedContinuation(key, candidate, key.branches.back()),
            orderedRestore(key, candidate),
        };
        auto production = grammar::Term::alt(
            id(key, "production.ordered"),
            {
            {id(key, "branch.ordered"), {}, candidateTerm(
                key, stages, candidate)},
            });
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
    const auto stay = id(key, "candidate.stay");
    const auto stayPhrase = phrase(key, stay, "establish");
    const auto stayStages = std::vector<Stage>{makeStage(key, stay, "stay", {
        operation::Anchor{sort::CenterId{key.centerRoot}},
        operation::Enter{sort::JinsId{key.jinsRoot}},
        operation::Begin{sort::PhraseId{stayPhrase}, phrase::Function{key.phraseQuestion}},
        operation::Place{
            sort::EventId{event(key, stay, "tonic")},
            sort::RoleId{key.roleTonic},
            motion::Direction::Start,
            sort::RegionId{key.regionRoot},
            std::nullopt,
        },
        operation::Emit{
            sort::CellId{key.cellEstablish},
            sort::FormulaId{key.formulaEstablish},
        },
        operation::Cadence{sort::FamilyId{key.cadenceLocal}, Rational(1), Rational(1)},
        operation::End{sort::PhraseId{stayPhrase}, phrase::Boundary::Closed},
        operation::sayr::Fulfill{sort::ObligationId{id(key, "obligation.establish")}},
        operation::sayr::Fulfill{sort::ObligationId{id(key, "obligation.settle")}},
    })};
    std::vector<grammar::Branch> branches{
        {id(key, "branch.stay"), {}, candidateTerm(key, stayStages, stay)},
    };
    for (const auto& branch : key.branches) {
        const auto candidate = id(key, "candidate." + branch.jins.name);
        branches.push_back({
            branch.route,
            {},
            candidateTerm(key, journey(key, candidate, branch), candidate),
        });
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
