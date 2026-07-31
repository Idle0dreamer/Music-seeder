#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"

namespace {

namespace kg = mq::kernel::grammar;

mq::kernel::Identity id(std::string name) {
    return {"test.phrase.scope", std::move(name), "1"};
}

kg::Term body(const mq::kernel::fixture::Set& fixture) {
    using namespace mq::kernel;
    const auto phrase = id("phrase");
    const auto begin = kg::Term::atom(
        id("begin"),
        operation::Begin{mq::kernel::sort::PhraseId{phrase},
            mq::kernel::phrase::Function{fixture.phrase.function},
        });
    const auto place = kg::Term::atom(
        id("place"),
        operation::Place{
            mq::kernel::sort::EventId{id("event")},
            mq::kernel::sort::RoleId{fixture.role.root},
            motion::Direction::Start,
            mq::kernel::sort::RegionId{fixture.region.root},
            std::nullopt,
        });
    const auto cadence = kg::Term::atom(
        id("cadence"),
        operation::Cadence{mq::kernel::sort::FamilyId{fixture.cadence}, Rational(1), Rational(1)});
    const auto end = kg::Term::atom(
        id("end"),
        operation::End{mq::kernel::sort::PhraseId{phrase}, mq::kernel::phrase::Boundary::Closed});
    return kg::Term::seq(
        id("head"),
        begin,
        kg::Term::seq(
            id("middle"),
            place,
            kg::Term::seq(id("tail"), cadence, end)));
}

} // namespace

void test::phrase::scope() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const kg::Evaluator evaluator(
        fixture.profile.shared,
        fixture.catalog);
    const auto phraseBody = body(fixture);
    state::Snapshot initial;
    initial.jins.active = mq::kernel::sort::JinsId{fixture.jins.root};

    const auto hidden = kg::Term::scope(
        id("hidden"),
        {id("scope.hidden"), kg::scope::Part::None},
        phraseBody);
    const auto local = evaluator.derive(hidden, initial);
    require(
        local.outcomes.size() == 1 &&
            local.outcomes.front().state.melody.history.empty() &&
            local.outcomes.front().state.phrase.completed.empty() &&
            local.outcomes.front().state.trace.events.size() == 4,
        "local phrase or melody state leaked through scope");

    const auto exposed = kg::Term::scope(
        id("exposed"),
        {
            id("scope.exposed"),
            kg::scope::Part::Melody | kg::scope::Part::Phrase,
        },
        phraseBody);
    const auto exported = evaluator.derive(exposed, initial);
    require(
        exported.outcomes.size() == 1 &&
            exported.outcomes.front().state.melody.history.size() == 1 &&
            exported.outcomes.front().state.phrase.completed.size() == 1,
        "explicit phrase and melody export lost span state");

    const auto dangling = kg::Term::scope(
        id("dangling"),
        {id("scope.dangling"), kg::scope::Part::Phrase},
        phraseBody);
    const auto rejected = evaluator.derive(dangling, initial);
    require(
        rejected.outcomes.empty() &&
            rejected.diagnostics.size() == 1 &&
            rejected.diagnostics.front().message ==
                "Phrase export requires Melody export",
        "phrase scope allowed dangling event identities");
}
