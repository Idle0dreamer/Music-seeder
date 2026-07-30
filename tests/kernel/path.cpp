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
        operation::Anchor{fixture.center.root},
        operation::Enter{fixture.jins.root},
        operation::Emphasize{fixture.role.ghammaz, Rational(2)},
        operation::Dwell{fixture.role.ghammaz, Rational(2)},
        operation::Emit{fixture.cell},
        operation::Begin{
            Identity{"test.path.phrase", "evidence", "1"},
            mq::kernel::phrase::Function{fixture.phrase.function},
        },
        operation::Place{
            Identity{"test.path.event", "cadence", "1"},
            fixture.role.root,
            motion::Direction::Start,
        },
        operation::Cadence{
            fixture.cadence,
            Rational(1),
            Rational(1),
        },
        operation::End{
            Identity{"test.path.phrase", "evidence", "1"},
            mq::kernel::phrase::Boundary::Closed,
        },
        operation::Tonicize{
            fixture.jins.branch,
            tonicization::Level::Internal,
        },
    };
    auto premature = evidence;
    premature.push_back(operation::Modulate{
        gated,
        fixture.center.branch,
        tonicization::Level::Internal,
    });
    const eval::Evaluator evaluator(*selected, graph);
    const auto rejected = evaluator.run({}, premature);
    require(
        !rejected && rejected.error().rule == "path.prerequisite",
        "path prerequisite was not enforced");

    auto complete = evidence;
    complete.push_back(operation::Modulate{
        first,
        fixture.center.branch,
        tonicization::Level::Internal,
    });
    complete.push_back(operation::Return{fixture.center.root});
    complete.push_back(operation::Modulate{
        gated,
        fixture.center.branch,
        tonicization::Level::Internal,
    });
    const auto accepted = evaluator.run({}, complete);
    require(accepted.has_value(), "legal path history was rejected");
    require(
        accepted->path.completed.contains(first) &&
            accepted->path.completed.contains(gated),
        "completed path history was not retained");

    const auto missing = eval::Evaluator(*selected).run({}, premature);
    require(
        !missing && missing.error().rule == "path.graph",
        "missing path graph silently fell back to permission alone");
}
