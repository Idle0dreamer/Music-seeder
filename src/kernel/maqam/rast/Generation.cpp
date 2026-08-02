#include "Internal.hpp"

#include "mq/kernel/generate/Stage.hpp"
#include "mq/kernel/operation/Operation.hpp"

namespace mq::kernel::maqam::rast::detail {
namespace {

using Stage = ::mq::kernel::generate::Stage;

Identity occurrence(const Identity& name, std::string suffix) {
    return id("occurrence." + name.name + "." + std::move(suffix));
}

Identity event(const Identity& name, std::string suffix) {
    return id("event." + name.name + "." + std::move(suffix));
}

Identity stage(const Identity& name, std::string suffix) {
    return id("stage." + name.name + "." + std::move(suffix));
}

Identity phrase(const Identity& name, std::string suffix) {
    return id("phrase." + name.name + "." + std::move(suffix));
}

Stage makeStage(
    const Identity& name,
    std::string suffix,
    std::vector<operation::Any> actions) {
    return {stage(name, std::move(suffix)), std::move(actions)};
}

Stage establishment(const Key& key, const Identity& candidate) {
    const auto p = phrase(candidate, "establish");
    const auto g = occurrence(candidate, "ascent");
    return makeStage(candidate, "establish", {
        operation::Anchor{sort::CenterId{key.centerRoot}},
        operation::Enter{sort::JinsId{key.jinsRoot}},
        operation::Begin{
            sort::PhraseId{p},
            phrase::Function{key.phraseQuestion},
        },
        operation::gesture::Begin{g, key.gestureAscent},
        operation::Place{
            sort::EventId{event(candidate, "tonic")},
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
    return makeStage(candidate, "development", {
        operation::Place{
            sort::EventId{event(candidate, "ghammaz")},
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
    const Identity& branch,
    const Identity& path,
    const Identity& travelObligation) {
    const auto p = phrase(candidate, "establish");
    return makeStage(candidate, "climax", {
        operation::Place{
            sort::EventId{event(candidate, "extension")},
            sort::RoleId{key.roleExtension},
            motion::Direction::Rise,
            sort::RegionId{key.regionUpper},
            sort::BaggageId{key.baggageExtension},
        },
        operation::Emit{
            sort::CellId{key.cellClimax},
            sort::FormulaId{key.formulaClimax},
        },
        operation::gesture::End{occurrence(candidate, "ascent")},
        operation::Cadence{
            sort::FamilyId{key.cadenceLocal},
            Rational(1),
            Rational(3, 4),
        },
        operation::End{
            sort::PhraseId{p},
            phrase::Boundary::Closed,
        },
        operation::sayr::Fulfill{
            sort::ObligationId{key.obligationEstablish},
        },
        operation::sayr::Fulfill{
            sort::ObligationId{key.obligationExpand},
        },
        operation::sayr::Fulfill{
            sort::ObligationId{key.obligationClimax},
        },
        operation::Tonicize{
            sort::JinsId{branch},
            tonicization::Level::Internal,
        },
        operation::Modulate{
            sort::PathId{path},
            sort::CenterId{key.centerUpper},
            tonicization::Level::Internal,
        },
        operation::sayr::Fulfill{
            sort::ObligationId{travelObligation},
        },
    });
}

Stage branchDescent(
    const Key& key,
    const Identity& candidate) {
    const auto p = phrase(candidate, "return");
    const auto descent = occurrence(candidate, "descent");
    return makeStage(candidate, "branch-descent", {
        operation::Begin{
            sort::PhraseId{p},
            phrase::Function{key.phraseResponse},
        },
        operation::gesture::Begin{descent, key.gestureDescent},
        operation::Place{
            sort::EventId{event(candidate, "branch")},
            sort::RoleId{key.roleUpper},
            motion::Direction::Fall,
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
    const Identity& restoreObligation) {
    const auto p = phrase(candidate, "return");
    const auto descent = occurrence(candidate, "descent");
    const auto resolution = occurrence(candidate, "resolution");
    return makeStage(candidate, "restore", {
        operation::gesture::End{descent},
        operation::Enter{sort::JinsId{key.jinsRoot}},
        operation::gesture::Begin{resolution, key.gestureResolution},
        operation::Place{
            sort::EventId{event(candidate, "tonic-return")},
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
        operation::End{
            sort::PhraseId{p},
            phrase::Boundary::Closed,
        },
        operation::Return{sort::CenterId{key.centerRoot}},
        operation::sayr::Fulfill{
            sort::ObligationId{restoreObligation},
        },
    });
}

grammar::Term actions(
    const Stage& value,
    const std::string& prefix) {
    auto result = grammar::Term::atom(
        id(prefix + ".atom.0"),
        value.actions.front());
    for (std::size_t index = 1; index < value.actions.size(); ++index) {
        result = grammar::Term::seq(
            id(prefix + ".seq." + std::to_string(index)),
            std::move(result),
            grammar::Term::atom(
                id(prefix + ".atom." + std::to_string(index)),
                value.actions[index]));
    }
    return result;
}

grammar::Term candidateTerm(
    const std::vector<Stage>& stages,
    const Identity& candidate) {
    auto body = grammar::Term::stage(
        id("term." + candidate.name + ".stage.0"),
        stages.front().identity,
        actions(stages.front(), "term." + candidate.name + ".stage.0"));
    for (std::size_t index = 1; index < stages.size(); ++index) {
        const auto prefix =
            "term." + candidate.name + ".stage." + std::to_string(index);
        body = grammar::Term::seq(
            id(prefix + ".seq"),
            std::move(body),
            grammar::Term::stage(
                id(prefix),
                stages[index].identity,
                actions(stages[index], prefix)));
    }
    return grammar::Term::candidate(
        id("term." + candidate.name + ".candidate"),
        candidate,
        std::move(body));
}

std::vector<Stage> journey(
    const Key& key,
    const Identity& candidate,
    const Identity& branch,
    const Identity& path,
    const Identity& travelObligation,
    const Identity& restoreObligation) {
    return {
        establishment(key, candidate),
        development(key, candidate),
        climax(key, candidate, branch, path, travelObligation),
        branchDescent(key, candidate),
        restore(key, candidate, restoreObligation),
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
    const auto tier = id("pitch.tier");
    const auto anchor = id("pitch.rule.anchor");
    const auto tonic = id("pitch.rule.tonic");
    const auto ghammaz = id("pitch.rule.ghammaz");
    const auto extension = id("pitch.rule.extension");
    const auto rastTarget = id("pitch.rule.branch.upper-rast");
    const auto nahawandTarget = id("pitch.rule.branch.nahawand");
    const auto returnTonic = id("pitch.rule.return-tonic");
    pf::Schema result;
    result.variables = {
        key.roleTonic,
        key.roleGhammaz,
        key.roleUpper,
        key.roleExtension,
    };
    result.tiers = {tier};
    result.rules.push_back(pf::Rule{
        pf::Guard{{}},
        pitch::Equation{
            anchor,
            {{key.roleTonic, Rational(1)}},
            {},
            "Rast tonic anchor; structural frequency authority",
        },
    });
    const auto addAim = [&](std::vector<pitch::field::Fact> facts,
                            Identity identity,
                            std::vector<pitch::Term> terms,
                            pitch::Expression target,
                            const char* provenance) {
        result.rules.push_back(pf::Rule{
            pf::Guard{std::move(facts)},
            pf::Aim{
                tier,
                {
                    std::move(identity),
                    std::move(terms),
                    std::move(target),
                    Rational(1),
                    provenance,
                },
            },
        });
    };
    addAim(
        {{key.keyRole, key.roleTonic},
         {key.keyMotion, key.motionStart},
         {key.keyRegion, key.regionRoot},
         {key.keyGesture, key.gestureAscent}},
        tonic,
        {{key.roleTonic, Rational(1)}},
        {},
        "Rast establishment departure");
    addAim(
        {{key.keyRole, key.roleGhammaz},
         {key.keyMotion, key.motionRise},
         {key.keyRegion, key.regionUpper},
         {key.keyGesture, key.gestureAscent}},
        ghammaz,
        {{key.roleGhammaz, Rational(1)}, {key.roleTonic, Rational(-1)}},
        pitch::Expression::ratio(3, 2),
        "Rast root jins ghammaz relation");
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
        pitch::Expression::ratio(2, 1),
        "Rast upper register closure");
    addAim(
        {{key.keyJins, key.jinsUpperRast},
         {key.keyRole, key.roleUpper},
         {key.keyMotion, key.motionFall},
         {key.keyRegion, key.regionUpper},
         {key.keyGesture, key.gestureDescent}},
        rastTarget,
        {{key.roleUpper, Rational(1)}, {key.roleTonic, Rational(-1)}},
        pitch::Expression::ratio(5, 3),
        "Rast upper-Rast branch target");
    addAim(
        {{key.keyJins, key.jinsUpperNahawand},
         {key.keyRole, key.roleUpper},
         {key.keyMotion, key.motionFall},
         {key.keyRegion, key.regionUpper},
         {key.keyGesture, key.gestureDescent}},
        nahawandTarget,
        {{key.roleUpper, Rational(1)}, {key.roleTonic, Rational(-1)}},
        pitch::Expression::ratio(3, 2),
        "Rast Nahawand branch target");
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
        "Rast return resolution target");
    result.needs = {{
        id("pitch.need"),
        {anchor, tonic, ghammaz, extension, rastTarget, nahawandTarget, returnTonic},
    }};
    return result;
}

} // namespace

std::expected<Generation, std::string> generation(const Key& key) {
    const auto stay = id("candidate.stay");
    const auto upperRast = id("candidate.upper-rast");
    const auto nahawand = id("candidate.nahawand");
    const auto stayPhrase = phrase(stay, "establish");
    const auto stayStages = std::vector<Stage>{makeStage(stay, "stay", {
        operation::Anchor{sort::CenterId{key.centerRoot}},
        operation::Enter{sort::JinsId{key.jinsRoot}},
        operation::Begin{
            sort::PhraseId{stayPhrase},
            phrase::Function{key.phraseQuestion},
        },
        operation::Place{
            sort::EventId{event(stay, "tonic")},
            sort::RoleId{key.roleTonic},
            motion::Direction::Start,
            sort::RegionId{key.regionRoot},
            std::nullopt,
        },
        operation::Emit{
            sort::CellId{key.cellEstablish},
            sort::FormulaId{key.formulaEstablish},
        },
        operation::Cadence{
            sort::FamilyId{key.cadenceLocal},
            Rational(1),
            Rational(1),
        },
        operation::End{
            sort::PhraseId{stayPhrase},
            phrase::Boundary::Closed,
        },
        operation::sayr::Fulfill{
            sort::ObligationId{key.obligationEstablish},
        },
        operation::sayr::Fulfill{
            sort::ObligationId{key.obligationSettle},
        },
    })};
    const auto journeys = std::array{
        journey(
            key,
            upperRast,
            key.jinsUpperRast,
            key.pathUpperRast,
            key.obligationTravelUpperRast,
            key.obligationRestoreUpperRast),
        journey(
            key,
            nahawand,
            key.jinsUpperNahawand,
            key.pathNahawand,
            key.obligationTravelNahawand,
            key.obligationRestoreNahawand),
    };
    auto production = grammar::Term::alt(
        id("production"),
        {
            {id("branch.stay"), {}, candidateTerm(stayStages, stay)},
            {id("branch.upper-rast"), {}, candidateTerm(journeys[0], upperRast)},
            {id("branch.nahawand"), {}, candidateTerm(journeys[1], nahawand)},
        });
    if (!production) {
        return std::unexpected(production.error());
    }
    return Generation{
        key.choice,
        std::move(*production),
        projection(key),
        schema(key),
        timing(),
    };
}

} // namespace mq::kernel::maqam::rast::detail
