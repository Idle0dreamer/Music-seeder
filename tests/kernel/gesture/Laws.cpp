#include "../Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/Set.hpp"

#include <algorithm>

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.gesture", std::move(name), "1"};
}

} // namespace

void test::gesture::laws() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const eval::Evaluator evaluator(
        fixture.profile.shared,
        fixture.catalog);
    state::Snapshot initial;
    initial.jins.active = mq::kernel::sort::JinsId{fixture.jins.root};

    const auto occurrence = id("occurrence");
    const operation::Place root{
        mq::kernel::sort::EventId{id("root")},
        mq::kernel::sort::RoleId{fixture.role.root},
        motion::Direction::Start,
        mq::kernel::sort::RegionId{fixture.region.root},
        std::nullopt,
    };
    const operation::Place ghammaz{
        mq::kernel::sort::EventId{id("ghammaz")},
        mq::kernel::sort::RoleId{fixture.role.ghammaz},
        motion::Direction::Rise,
        mq::kernel::sort::RegionId{fixture.region.upper},
        std::nullopt,
    };
    const operation::Place baggage{
        mq::kernel::sort::EventId{id("baggage")},
        mq::kernel::sort::RoleId{fixture.role.baggage},
        motion::Direction::Rise,
        mq::kernel::sort::RegionId{fixture.region.upper},
        fixture.baggage,
    };
    const std::vector<operation::Any> open{
        operation::gesture::Begin{
            occurrence,
            fixture.gesture.ascent,
        },
        root,
    };
    const auto first = evaluator.run(initial, open);
    require(
        first && first->gesture.active &&
            first->gesture.active->next == 1,
        "first gesture step did not advance exactly once");

    const auto early = evaluator.run(
        *first,
        std::vector<operation::Any>{
            operation::gesture::End{occurrence},
        });
    require(
        !early && early.error().rule == "gesture.incomplete",
        "gesture ended before its ordered steps were complete");

    auto wrong = ghammaz;
    wrong.region = mq::kernel::sort::RegionId{fixture.region.root};
    const auto mismatch = evaluator.run(
        *first,
        std::vector<operation::Any>{wrong});
    require(
        !mismatch && mismatch.error().rule == "gesture.step",
        "gesture accepted an out-of-order step domain");

    const auto second = evaluator.run(
        *first,
        std::vector<operation::Any>{ghammaz});
    require(
        second && second->gesture.active &&
            second->gesture.active->next == 2,
        "second gesture step did not advance exactly once");
    const auto third = evaluator.run(
        *second,
        std::vector<operation::Any>{baggage});
    require(
        third && third->gesture.active &&
            third->gesture.active->next == 3 &&
            third->evidence.amount.at(evidence::Kind::Baggage) ==
                Rational(1) &&
            third->evidence.amount.at(evidence::Kind::Register) ==
                Rational(3),
        "descriptor event evidence was not accumulated exactly");

    const auto completed = evaluator.run(
        *third,
        std::vector<operation::Any>{
            operation::gesture::End{occurrence},
        });
    require(
        completed && !completed->gesture.active &&
            completed->gesture.completed.size() == 1 &&
            completed->gesture.completed.front().first == root.event &&
            completed->gesture.completed.front().last == baggage.event &&
            std::ranges::all_of(
                completed->melody.history,
                [&](const auto& event) {
                    return event.gesture &&
                           event.gesture->occurrence == occurrence &&
                           event.gesture->family ==
                               fixture.gesture.ascent;
                }),
        "completed gesture lost its exact event span or event context");

    const auto plain = evaluator.run(
        initial,
        std::vector<operation::Any>{root});
    require(plain.has_value(), plain ? "" : plain.error().message);
    const auto detached = evaluator.run(
        *plain,
        std::vector<operation::Any>{baggage});
    require(
        !detached && detached.error().rule == "jins.baggage",
        "gesture-bound baggage was accepted outside its gesture");

    const eval::Evaluator narrowed(
        fixture.profile.regional.b,
        fixture.catalog);
    const std::vector<operation::Any> full{
        operation::gesture::Begin{
            id("narrowed"),
            fixture.gesture.ascent,
        },
        operation::Place{
        mq::kernel::sort::EventId{id("narrowed.root")},
        mq::kernel::sort::RoleId{fixture.role.root},
        motion::Direction::Start,
        mq::kernel::sort::RegionId{fixture.region.root},
        std::nullopt,
    },
        operation::Place{
        mq::kernel::sort::EventId{id("narrowed.ghammaz")},
        mq::kernel::sort::RoleId{fixture.role.ghammaz},
        motion::Direction::Rise,
        mq::kernel::sort::RegionId{fixture.region.upper},
        std::nullopt,
    },
        operation::Place{
        mq::kernel::sort::EventId{id("narrowed.baggage")},
        mq::kernel::sort::RoleId{fixture.role.baggage},
        motion::Direction::Rise,
        mq::kernel::sort::RegionId{fixture.region.upper},
        fixture.baggage,
    },
    };
    const auto forbidden = narrowed.run(initial, full);
    require(
        !forbidden && forbidden.error().rule == "allow.baggage",
        "profile narrowing failed to remove descriptor-legal baggage");
}
