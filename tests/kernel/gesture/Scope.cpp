#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"

namespace {

namespace kg = mq::kernel::grammar;

mq::kernel::Identity id(std::string name) {
    return {"test.gesture.scope", std::move(name), "1"};
}

kg::Term body(const mq::kernel::fixture::Set& fixture) {
    using namespace mq::kernel;
    const auto occurrence = id("occurrence");
    const std::vector<operation::Any> actions{
        operation::gesture::Begin{
            occurrence,
            fixture.gesture.ascent,
        },
        operation::Place{
            sort::EventId{id("event.root")},
            sort::RoleId{fixture.role.root},
            motion::Direction::Start,
            sort::RegionId{fixture.region.root},
            std::nullopt,
        },
        operation::Place{
            sort::EventId{id("event.ghammaz")},
            sort::RoleId{fixture.role.ghammaz},
            motion::Direction::Rise,
            sort::RegionId{fixture.region.upper},
            std::nullopt,
        },
        operation::Place{
            sort::EventId{id("event.baggage")},
            sort::RoleId{fixture.role.baggage},
            motion::Direction::Rise,
            sort::RegionId{fixture.region.upper},
            sort::BaggageId{fixture.baggage},
        },
        operation::gesture::End{occurrence},
    };
    auto term = kg::Term::atom(id("atom.0"), actions.front());
    for (std::size_t index = 1; index < actions.size(); ++index) {
        term = kg::Term::seq(
            id("seq." + std::to_string(index)),
            std::move(term),
            kg::Term::atom(
                id("atom." + std::to_string(index)),
                actions[index]));
    }
    return term;
}

} // namespace

void test::gesture::scope() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const kg::Evaluator evaluator(
        fixture.profile.shared,
        fixture.catalog);
    state::Snapshot initial;
    initial.jins.active = mq::kernel::sort::JinsId{fixture.jins.root};
    const auto gesture = body(fixture);

    const auto hidden = evaluator.derive(
        kg::Term::scope(
            id("hidden"),
            {id("policy.hidden"), kg::scope::Part::None},
            gesture),
        initial);
    require(
        hidden.outcomes.size() == 1 &&
            hidden.outcomes.front().state.melody.history.empty() &&
            hidden.outcomes.front().state.gesture.completed.empty() &&
            hidden.outcomes.front().state.trace.events.size() == 5,
        "local gesture or melody state leaked through scope");

    const auto exported = evaluator.derive(
        kg::Term::scope(
            id("exported"),
            {
                id("policy.exported"),
                kg::scope::Part::Melody | kg::scope::Part::Gesture,
            },
            gesture),
        initial);
    require(
        exported.outcomes.size() == 1 &&
            exported.outcomes.front().state.melody.history.size() == 3 &&
            exported.outcomes.front().state.gesture.completed.size() == 1,
        "explicit gesture and melody export lost ordered span state");

    const auto dangling = evaluator.derive(
        kg::Term::scope(
            id("dangling"),
            {id("policy.dangling"), kg::scope::Part::Gesture},
            gesture),
        initial);
    require(
        dangling.outcomes.empty() &&
            dangling.diagnostics.size() == 1 &&
            dangling.diagnostics.front().message ==
                "Gesture export requires Melody export",
        "gesture scope allowed dangling event identities");
}
