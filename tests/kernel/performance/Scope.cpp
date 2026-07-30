#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.event.scope", std::move(name), "1"};
}

} // namespace

void test::event::scope() {
    using namespace mq::kernel;
    namespace kg = mq::kernel::grammar;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const kg::Evaluator evaluator(fixture.profile.shared);
    const auto event = kg::Term::atom(
        id("place"),
        operation::Place{
            id("event"),
            fixture.role.root,
            motion::Direction::Start,
        });

    const auto local = kg::Term::scope(
        id("local"),
        {id("scope.local"), kg::scope::Part::None},
        event);
    const auto hidden = evaluator.derive(local);
    require(
        hidden.outcomes.size() == 1 &&
            hidden.outcomes.front().state.melody.history.empty() &&
            !hidden.outcomes.front().state.melody.current &&
            hidden.outcomes.front().state.trace.events.size() == 1,
        "non-exported structural event leaked or erased trace evidence");

    const auto exported = kg::Term::scope(
        id("exported"),
        {id("scope.exported"), kg::scope::Part::Melody},
        event);
    const auto visible = evaluator.derive(exported);
    require(
        visible.outcomes.size() == 1 &&
            visible.outcomes.front().state.melody.history.size() == 1 &&
            visible.outcomes.front().state.melody.current,
        "explicit Melody scope export did not preserve event state");
}
