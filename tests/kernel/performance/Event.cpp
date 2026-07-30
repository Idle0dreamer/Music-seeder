#include "../Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/Set.hpp"

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.event", std::move(name), "1"};
}

} // namespace

void test::event::laws() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const eval::Evaluator evaluator(fixture.profile.shared);

    const operation::Place first{
        id("first"),
        fixture.role.root,
        motion::Direction::Start,
    };
    const std::vector<operation::Any> begin{first};
    const auto started = evaluator.run({}, begin);
    require(
        started &&
            started->melody.current ==
                performance::Event{
                    first.event,
                    first.role,
                    first.direction,
                } &&
            started->melody.history.size() == 1,
        "first structural event was not recorded exactly once");

    const operation::Place second{
        id("second"),
        fixture.role.ghammaz,
        motion::Direction::Rise,
    };
    const std::vector<operation::Any> continueWith{second};
    const auto continued = evaluator.run(*started, continueWith);
    require(
        continued &&
            continued->melody.history.size() == 2 &&
            continued->melody.history.back().identity == second.event &&
            continued->trace.events.back().operation == "Place",
        "continuing structural event lost identity, history, or trace");

    const std::vector<operation::Any> wrongFirst{
        operation::Place{
            id("wrong-first"),
            fixture.role.root,
            motion::Direction::Rise,
        },
    };
    const auto rejectedFirst = evaluator.run({}, wrongFirst);
    require(
        !rejectedFirst &&
            rejectedFirst.error().rule == "event.direction",
        "first event accepted non-Start direction");

    const std::vector<operation::Any> restarted{
        operation::Place{
            id("restart"),
            fixture.role.root,
            motion::Direction::Start,
        },
    };
    const auto rejectedRestart = evaluator.run(*started, restarted);
    require(
        !rejectedRestart &&
            rejectedRestart.error().rule == "event.direction",
        "continuing event accepted Start direction");

    const std::vector<operation::Any> repeated{
        operation::Place{
            first.event,
            fixture.role.root,
            motion::Direction::Same,
        },
    };
    const auto duplicate = evaluator.run(*started, repeated);
    require(
        !duplicate && duplicate.error().rule == "event.identity",
        "duplicate structural event identity was accepted");

    const std::vector<operation::Any> forbidden{
        operation::Place{
            id("forbidden"),
            id("unknown-role"),
            motion::Direction::Start,
        },
    };
    const auto denied = evaluator.run({}, forbidden);
    require(
        !denied && denied.error().rule == "allow.place",
        "profile-disallowed structural role was accepted");
}
