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
        value.schema);
    require(
        first &&
            first->plan.targets.size() == 1 &&
            first->plan.targets.back().center == pitch::Expression{} &&
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
        first->plan);
    require(
        second &&
            second->plan.targets.size() == 2 &&
            second->plan.targets.back().center ==
                pitch::Expression::ratio(4, 3) &&
            second->direction &&
            second->direction->relation ==
                pitch::order::Relation::Greater,
        "rising request lost exact target or order certificate");

    const auto missing = pr::run(
        *secondState,
        value.projection,
        value.schema);
    require(
        !missing && missing.error().code == pr::Error::Code::History,
        "pitch request accepted a missing performance-plan prefix");

    pr::Limits shortHistory;
    shortHistory.history = 1;
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
        first->plan);
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
        first->plan);
    require(
        !opposed && opposed.error().code == pr::Error::Code::Direction,
        "calculated fall was accepted as intended rise");

    pr::Limits limited;
    limited.order.exponent = 1;
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
