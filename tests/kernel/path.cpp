#include "Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/Set.hpp"

#include <vector>

void test::path() {
    using namespace mq::kernel;
    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;

    const Identity first{"test.path", "first", "1"};
    const Identity gated{"test.path", "gated", "1"};
    const profile::Patch exception{
        profile::Patch::Action::AddException,
        "allow.modulate",
        profile::Rule{
            profile::Domain{first, gated},
            {"test:neutral-paths"},
        },
    };
    const auto selected = profile::reconstruct(
        "test.paths",
        {fixture.profile.shared},
        {exception});
    require(selected.has_value(), selected.error_or("profile failed"));

    path::Graph graph;
    require(
        graph.add({
            first,
            fixture.center.root,
            fixture.center.branch,
            tonicization::Level::Internal,
            {},
            "test:first",
        }).has_value(),
        "first path rule failed");
    require(
        graph.add({
            gated,
            fixture.center.root,
            fixture.center.branch,
            tonicization::Level::Internal,
            {first},
            "test:gated",
        }).has_value(),
        "gated path rule failed");

    const std::vector<operation::Any> evidence{
        operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}},
        operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
        operation::Emphasize{mq::kernel::sort::RoleId{fixture.role.ghammaz}, Rational(2)},
        operation::Dwell{mq::kernel::sort::RoleId{fixture.role.ghammaz}, Rational(2)},
        operation::Emit{mq::kernel::sort::CellId{fixture.cell}},
        operation::Begin{mq::kernel::sort::PhraseId{Identity{"test.path.phrase", "evidence", "1"}},
            mq::kernel::phrase::Function{fixture.phrase.function},
        },
        operation::Place{
                    mq::kernel::sort::EventId{Identity{"test.path.event", "cadence", "1"}},
                    mq::kernel::sort::RoleId{fixture.role.root},
                    motion::Direction::Start,
                    mq::kernel::sort::RegionId{fixture.region.root},
                    std::nullopt,
                },
        operation::Cadence{mq::kernel::sort::FamilyId{fixture.cadence},
            Rational(1),
            Rational(1),
        },
        operation::End{mq::kernel::sort::PhraseId{Identity{"test.path.phrase", "evidence", "1"}},
            mq::kernel::phrase::Boundary::Closed,
        },
        operation::Tonicize{mq::kernel::sort::JinsId{fixture.jins.branch},
            tonicization::Level::Internal,
        },
    };
    auto premature = evidence;
    premature.push_back(operation::Modulate{mq::kernel::sort::PathId{gated}, mq::kernel::sort::CenterId{fixture.center.branch}, 
        tonicization::Level::Internal,
    });
    const eval::Evaluator evaluator(
        *selected,
        fixture.catalog,
        graph);
    const auto rejected = evaluator.run({}, premature);
    require(
        !rejected && rejected.error().rule == "path.prerequisite",
        "path prerequisite was not enforced");

    auto complete = evidence;
    complete.push_back(operation::Modulate{mq::kernel::sort::PathId{first}, mq::kernel::sort::CenterId{fixture.center.branch}, 
        tonicization::Level::Internal,
    });
    complete.push_back(operation::Return{mq::kernel::sort::CenterId{fixture.center.root}});
    complete.push_back(operation::Modulate{mq::kernel::sort::PathId{gated}, mq::kernel::sort::CenterId{fixture.center.branch}, 
        tonicization::Level::Internal,
    });
    const auto accepted = evaluator.run({}, complete);
    require(accepted.has_value(), "legal path history was rejected");
    require(
        accepted->path.completed.contains(sort::PathId{first}) &&
            accepted->path.completed.contains(sort::PathId{gated}),
        "completed path history was not retained");

    const auto missing =
        eval::Evaluator(*selected, fixture.catalog).run({}, premature);
    require(
        !missing && missing.error().rule == "path.graph",
        "missing path graph silently fell back to permission alone");
}
