#include "../../Test.hpp"
#include "Fixture.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/pitch/request/Run.hpp"

#include <variant>

void test::request::run() {
    using namespace mq::kernel;
    namespace pr = pitch::request;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const auto value = make(fixture);
    const eval::Evaluator evaluator(
        fixture.profile.shared,
        fixture.catalog);
    state::Snapshot initial;
    initial.jins.active = mq::kernel::sort::JinsId{fixture.jins.root};
    const auto timing = test::timing_profile();

    const std::vector<operation::Any> firstProgram{
        operation::Place{
                    mq::kernel::sort::EventId{value.first},
                    mq::kernel::sort::RoleId{fixture.role.root},
                    motion::Direction::Start,
                    mq::kernel::sort::RegionId{fixture.region.root},
                    std::nullopt,
                },
    };
    const auto firstState = evaluator.run(initial, firstProgram);
    require(
        firstState.has_value(),
        firstState ? "" : firstState.error().message);
    const auto first = pr::run(
        *firstState,
        value.projection,
        value.schema,
        {},
        pr::Limits{.timing = timing});
    require(
        first &&
            first->plan.events.size() == 1 &&
            first->plan.events.back().target.center == pitch::Expression{} &&
            first->plan.events.back().onset == Rational(0) &&
            first->plan.events.back().duration == timing.start.duration &&
            first->plan.well_formed() &&
            !first->direction,
        "first calculated performance target is incorrect");

    const std::vector<operation::Any> secondProgram{
        operation::Place{
                    mq::kernel::sort::EventId{value.second},
                    mq::kernel::sort::RoleId{fixture.role.ghammaz},
                    motion::Direction::Rise,
                    mq::kernel::sort::RegionId{fixture.region.upper},
                    std::nullopt,
                },
    };
    const auto secondState = evaluator.run(*firstState, secondProgram);
    require(
        secondState.has_value(),
        secondState ? "" : secondState.error().message);
    const auto second = pr::run(
        *secondState,
        value.projection,
        value.schema,
        first->plan,
        pr::Limits{.timing = timing});
    require(
        second &&
            second->plan.events.size() == 2 &&
            second->plan.events.back().target.center ==
                pitch::Expression::ratio(4, 3) &&
            second->plan.events.back().onset == timing.start.duration &&
            second->plan.events.back().duration == timing.rise.duration &&
            second->plan.events.back().contour &&
            second->plan.events.back().contour->well_formed() &&
            second->plan.events.back().contour->points.front().offset ==
                pitch::Expression::ratio(3, 4) &&
            second->plan.events.back().ornament &&
            second->plan.events.back().ornament->kind ==
                performance::OrnamentKind::Approach &&
            second->plan.well_formed() &&
            second->direction &&
            second->direction->relation ==
                pitch::order::Relation::Greater,
        "rising request lost exact target or order certificate");

    auto variableTiming = first->plan;
    variableTiming.events.front().duration = timing.start.duration * Rational(2);
    variableTiming.events.front().intensity = timing.start.intensity;
    variableTiming.events.front().articulation =
        performance::Articulation::Connected;
    const auto varied = pr::run(
        *secondState,
        value.projection,
        value.schema,
        variableTiming,
        pr::Limits{.timing = timing});
    require(
        varied &&
            varied->plan.events.back().onset ==
                timing.start.duration * Rational(2) &&
            varied->plan.events.front().intensity == timing.start.intensity &&
            varied->plan.events.front().articulation ==
                performance::Articulation::Connected &&
            varied->plan.well_formed(),
        "timed plan did not derive onset from prior exact duration");

    const auto missing = pr::run(
        *secondState,
        value.projection,
        value.schema,
        {},
        pr::Limits{.timing = timing});
    require(
        !missing && missing.error().code == pr::Error::Code::History,
        "pitch request accepted a missing performance-plan prefix");

    auto brokenTiming = first->plan;
    brokenTiming.events.front().duration = Rational(0);
    const auto rejectedTiming = pr::run(
        *secondState,
        value.projection,
        value.schema,
        brokenTiming,
        pr::Limits{.timing = timing});
    require(
        !rejectedTiming &&
            rejectedTiming.error().code == pr::Error::Code::Plan,
        "pitch request accepted an invalid timed-plan prefix");

    pr::Limits shortHistory;
    shortHistory.history = 1;
    shortHistory.timing = timing;
    const auto oversized = pr::run(
        *secondState,
        value.projection,
        value.schema,
        first->plan,
        shortHistory);
    require(
        !oversized && oversized.error().code == pr::Error::Code::Count,
        "pitch request history budget exhaustion did not fail");

    auto malformed = *secondState;
    malformed.melody.history.back().identity = value.first;
    malformed.melody.current = malformed.melody.history.back();
    const auto corrupt = pr::run(
        malformed,
        value.projection,
        value.schema,
        first->plan,
        pr::Limits{.timing = timing});
    require(
        !corrupt && corrupt.error().code == pr::Error::Code::State,
        "pitch request accepted malformed structural event history");

    auto contradictory = value.schema;
    for (auto& rule : contradictory.rules) {
        if (std::holds_alternative<pitch::field::Aim>(rule.effect)) {
            auto& aim = std::get<pitch::field::Aim>(rule.effect);
            if (aim.target.identity == value.rise) {
                aim.target.target =
                    pitch::Expression::cents(Rational(-10));
            }
        }
    }
    const auto opposed = pr::run(
        *secondState,
        value.projection,
        contradictory,
        first->plan,
        pr::Limits{.timing = timing});
    require(
        !opposed && opposed.error().code == pr::Error::Code::Direction,
        "calculated fall was accepted as intended rise");

    pr::Limits limited;
    limited.order.exponent = 1;
    limited.timing = timing;
    const auto exhausted = pr::run(
        *secondState,
        value.projection,
        value.schema,
        first->plan,
        limited);
    require(
        !exhausted && exhausted.error().code == pr::Error::Code::Order,
        "motion proof budget exhaustion did not fail explicitly");
}
