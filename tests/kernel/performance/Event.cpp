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
    const eval::Evaluator evaluator(
        fixture.profile.shared,
        fixture.catalog);
    state::Snapshot initial;
    initial.jins.active = fixture.jins.root;

    const operation::Place first{
        id("first"),
        fixture.role.root,
        motion::Direction::Start,
        fixture.region.root,
        std::nullopt,
    };
    const std::vector<operation::Any> begin{first};
    const auto started = evaluator.run(initial, begin);
    require(
        started &&
            started->melody.current ==
                performance::Event{
                    first.event.identity,
                    first.role.identity,
                    first.direction,
                    first.region.identity,
                    first.baggage ? std::optional(first.baggage->identity) : std::nullopt,
                    std::nullopt,
                } &&
            started->melody.history.size() == 1,
        "first structural event was not recorded exactly once");

    const operation::Place second{
        id("second"),
        fixture.role.ghammaz,
        motion::Direction::Rise,
        fixture.region.upper,
        std::nullopt,
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
            fixture.region.root,
            std::nullopt,
        },
    };
    const auto rejectedFirst = evaluator.run(initial, wrongFirst);
    require(
        !rejectedFirst &&
            rejectedFirst.error().rule == "event.direction",
        "first event accepted non-Start direction");

    const std::vector<operation::Any> restarted{
        operation::Place{
            id("restart"),
            fixture.role.root,
            motion::Direction::Start,
            fixture.region.root,
            std::nullopt,
        },
    };
    const auto rejectedRestart = evaluator.run(*started, restarted);
    require(
        !rejectedRestart &&
            rejectedRestart.error().rule == "event.direction",
        "continuing event accepted Start direction");

    const std::vector<operation::Any> repeated{
        operation::Place{
            first.event.identity,
            fixture.role.root,
            motion::Direction::Same,
            fixture.region.root,
            std::nullopt,
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
            fixture.region.root,
            std::nullopt,
        },
    };
    const auto denied = evaluator.run(initial, forbidden);
    require(
        !denied && denied.error().rule == "jins.role",
        "descriptor-disallowed structural role was accepted");

    const std::vector<operation::Any> outside{
        operation::Place{
            id("outside-register"),
            fixture.role.root,
            motion::Direction::Start,
            id("unknown-register"),
            std::nullopt,
        },
    };
    const auto unregistered = evaluator.run(initial, outside);
    require(
        !unregistered && unregistered.error().rule == "jins.register",
        "descriptor-disallowed register region was accepted");
}
