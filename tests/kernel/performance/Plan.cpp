#include "../Test.hpp"

#include "mq/kernel/performance/Plan.hpp"
#include "mq/kernel/performance/Timing.hpp"

#include <cstdint>

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
    plan.append(
        target,
        Rational(2),
        Rational(1),
        performance::Articulation::Neutral);
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
        id("ornament.oscillation"),
        mq::kernel::performance::OrnamentKind::Oscillation,
        pitch::Expression::ratio(1, 100),
        Rational(2),
        "test:exact-ornament-law",
    };
    require(plan.well_formed(), "exact ornament intent was rejected");

    plan.events.front().contour->points.back().position = Rational(3, 4);
    require(
        !plan.well_formed(),
        "unterminated pitch contour was accepted");
    plan.events.front().contour->points.back().position = Rational(1);

    auto timing = test::timing_profile();
    require(timing.well_formed(), "timing profile was rejected");
    auto invalid = timing;
    invalid.rise.duration = mq::kernel::Rational(0);
    require(!invalid.well_formed(), "invalid timing policy was accepted");

    plan.events.front().release = performance::Release{
        Rational(1, 4),
        performance::Articulation::Detached,
    };
    plan.append_pause(
        Rational(1, 4),
        id("pause.cadence"),
        "test:cadence-pause");
    require(plan.well_formed(), "explicit release or pause was rejected");

    auto variable = timing;
    variable.duration_variation = Rational(1, 4);
    variable.intensity_variation = Rational(1, 4);
    bool changed = false;
    for (std::uint64_t seed = 0; seed < 4; ++seed) {
        const auto resolved = variable.resolve(
            motion::Direction::Rise,
            performance::Context{3, false, false, false, seed});
        if (resolved.duration != timing.rise.duration ||
            resolved.intensity != timing.rise.intensity) {
            changed = true;
        }
    }
    require(changed, "seeded performer timing variation was not applied");
}
