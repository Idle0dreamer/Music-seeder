#include "../Test.hpp"

#include "mq/kernel/performance/Plan.hpp"

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.performance", std::move(name), "1"};
}

} // namespace

void test::timed::plan() {
    using namespace mq::kernel;

    const mq::kernel::performance::Target target{
        mq::kernel::performance::Event{
            id("event"),
            id("role"),
            motion::Direction::Start,
            id("region"),
            std::nullopt,
            std::nullopt,
        },
        pitch::Expression::ratio(1, 1),
    };
    mq::kernel::performance::Plan plan;
    plan.append(target, Rational(2), Rational(1));
    require(plan.well_formed(), "stationary timed plan was rejected");

    plan.events.front().contour = mq::kernel::performance::PitchContour{
        {
            {Rational(0), pitch::Expression::ratio(1, 1)},
            {Rational(1, 2), pitch::Expression::ratio(9, 8)},
            {Rational(1), pitch::Expression::ratio(1, 1)},
        },
    };
    require(
        plan.well_formed(),
        "exact continuous pitch contour was rejected");

    plan.events.front().ornament = mq::kernel::performance::Ornament{
        mq::kernel::performance::OrnamentKind::Oscillation,
        pitch::Expression::ratio(1, 100),
        Rational(2),
    };
    require(plan.well_formed(), "exact ornament intent was rejected");

    plan.events.front().contour->points.back().position = Rational(3, 4);
    require(
        !plan.well_formed(),
        "unterminated pitch contour was accepted");
}
