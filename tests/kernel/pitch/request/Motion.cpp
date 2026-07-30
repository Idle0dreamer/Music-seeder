#include "../../Test.hpp"
#include "Fixture.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/pitch/request/Run.hpp"

namespace {

std::expected<mq::kernel::state::Snapshot, mq::kernel::eval::Violation> place(
    const mq::kernel::eval::Evaluator& evaluator,
    mq::kernel::state::Snapshot state,
    const mq::kernel::Identity& event,
    const mq::kernel::Identity& role,
    mq::kernel::motion::Direction direction) {
    const std::vector<mq::kernel::operation::Any> program{
        mq::kernel::operation::Place{
            event,
            role,
            direction,
        },
    };
    return evaluator.run(std::move(state), program);
}

} // namespace

void test::request::motion() {
    using namespace mq::kernel;
    namespace pr = pitch::request;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const auto value = make(fixture);
    const eval::Evaluator evaluator(fixture.profile.shared);

    auto firstState = place(
        evaluator,
        {},
        value.first,
        fixture.role.root,
        motion::Direction::Start);
    require(
        firstState.has_value(),
        firstState ? "" : firstState.error().message);
    auto first = pr::run(
        *firstState,
        value.projection,
        value.schema);
    require(
        first.has_value(),
        first ? "" : first.error().message);

    auto secondState = place(
        evaluator,
        *firstState,
        value.second,
        fixture.role.ghammaz,
        motion::Direction::Rise);
    require(
        secondState.has_value(),
        secondState ? "" : secondState.error().message);
    auto second = pr::run(
        *secondState,
        value.projection,
        value.schema,
        first->plan);
    require(
        second.has_value(),
        second ? "" : second.error().message);

    auto thirdState = place(
        evaluator,
        *secondState,
        value.third,
        fixture.role.ghammaz,
        motion::Direction::Same);
    require(
        thirdState.has_value(),
        thirdState ? "" : thirdState.error().message);
    auto third = pr::run(
        *thirdState,
        value.projection,
        value.schema,
        second->plan);
    require(
        third &&
            third->direction &&
            third->direction->relation ==
                pitch::order::Relation::Equal &&
            third->plan.targets.back().center ==
                pitch::Expression::ratio(4, 3),
        "Same motion lacks an exact equality certificate");

    auto fourthState = place(
        evaluator,
        *thirdState,
        value.fourth,
        fixture.role.root,
        motion::Direction::Fall);
    require(
        fourthState.has_value(),
        fourthState ? "" : fourthState.error().message);
    auto fourth = pr::run(
        *fourthState,
        value.projection,
        value.schema,
        third->plan);
    require(
        fourth &&
            fourth->direction &&
            fourth->direction->relation ==
                pitch::order::Relation::Less &&
            fourth->plan.targets.back().center == pitch::Expression{},
        "Fall motion lacks an exact less-than certificate");
}
