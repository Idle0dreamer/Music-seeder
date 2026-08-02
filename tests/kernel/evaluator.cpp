#include "Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/Set.hpp"

#include <vector>

void test::evaluator() {
    using namespace mq::kernel;
    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;

    struct Evaluators {
        eval::Evaluator shared;
        struct {
            eval::Evaluator a;
            eval::Evaluator b;
        } regional;
    };
    const Evaluators evaluator{
        eval::Evaluator(
            fixture.profile.shared,
            fixture.catalog,
            fixture.path.graph),
        {
            eval::Evaluator(
                fixture.profile.regional.a,
                fixture.catalog,
                fixture.path.graph),
            eval::Evaluator(
                fixture.profile.regional.b,
                fixture.catalog,
                fixture.path.graph),
        },
    };

    const std::vector<operation::Any> premature{
        operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}},
        operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
        operation::Tonicize{mq::kernel::sort::JinsId{fixture.jins.branch},
            tonicization::Level::Internal,
        },
    };
    const auto rejected = evaluator.shared.run({}, premature);
    require(!rejected, "one entered jins incorrectly proved tonicization");
    require(
        rejected.error().rule == "threshold.internal.dwell",
        "tonicization failure did not identify missing evidence");

    const std::vector<operation::Any> established{
        operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}},
        operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
        operation::Emphasize{mq::kernel::sort::RoleId{fixture.role.ghammaz}, Rational(2)},
        operation::Dwell{mq::kernel::sort::RoleId{fixture.role.ghammaz}, Rational(2)},
        operation::Begin{mq::kernel::sort::PhraseId{Identity{"test.phrase", "establish", "1"}},
            mq::kernel::phrase::Function{fixture.phrase.function},
        },
        operation::Place{
                    mq::kernel::sort::EventId{Identity{"test.event", "cadence", "1"}},
                    mq::kernel::sort::RoleId{fixture.role.root},
                    motion::Direction::Start,
                    mq::kernel::sort::RegionId{fixture.region.root},
                    std::nullopt,
                },
        operation::Emit{
            mq::kernel::sort::CellId{fixture.cell},
            std::nullopt,
        },
        operation::Cadence{mq::kernel::sort::FamilyId{fixture.cadence},
            Rational(1),
            Rational(1),
        },
        operation::End{mq::kernel::sort::PhraseId{Identity{"test.phrase", "establish", "1"}},
            mq::kernel::phrase::Boundary::Closed,
        },
        operation::Tonicize{mq::kernel::sort::JinsId{fixture.jins.branch},
            tonicization::Level::Internal,
        },
        operation::Modulate{mq::kernel::sort::PathId{fixture.path.direct}, mq::kernel::sort::CenterId{fixture.center.branch}, 
            tonicization::Level::Internal,
        },
        operation::Return{mq::kernel::sort::CenterId{fixture.center.root}},
    };
    const auto accepted = evaluator.shared.run({}, established);
    require(accepted.has_value(), "legal neutral program was rejected");
    require(
        accepted->center.stack.size() == 1 &&
            accepted->center.stack.back().identity == fixture.center.root &&
            accepted->cell.owners.size() == 1 &&
            accepted->cell.owners.begin()->second.cell.identity == fixture.cell &&
            !accepted->cell.owners.begin()->second.variation,
        "return did not restore the established center");

    const std::vector<operation::Any> unbound{
        operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}},
        operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
        operation::Emit{
            mq::kernel::sort::CellId{fixture.cell},
            std::nullopt,
        },
    };
    const auto rejectedCell = evaluator.shared.run({}, unbound);
    require(
        !rejectedCell && rejectedCell.error().rule == "cell.event",
        "cell emission without an event was accepted");

    const std::vector<operation::Any> unapprovedVariation{
        operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}},
        operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
        operation::Place{
            mq::kernel::sort::EventId{Identity{"test.event", "variation", "1"}},
            mq::kernel::sort::RoleId{fixture.role.root},
            motion::Direction::Start,
            mq::kernel::sort::RegionId{fixture.region.root},
            std::nullopt,
        },
        operation::Emit{
            mq::kernel::sort::CellId{fixture.cell},
            mq::kernel::sort::FormulaId{Identity{
                "test.formula", "unapproved", "1"}},
        },
    };
    const auto rejectedVariation = evaluator.shared.run(
        {}, unapprovedVariation);
    require(
        !rejectedVariation &&
            rejectedVariation.error().rule == "allow.variation",
        "unapproved cell variation was accepted");

    const auto a = evaluator.regional.a.run({}, established);
    require(
        !a &&
            a.error().rule ==
                "threshold.internal.emphasis",
        "regional evidence threshold was not reconstructed");

    auto stronger = established;
    std::get<operation::Emphasize>(stronger[2]).amount = Rational(3);
    require(
        evaluator.regional.a.run({}, stronger).has_value(),
        "regional evidence threshold rejected sufficient evidence");

    const auto b = evaluator.regional.b.run({}, stronger);
    require(
        !b &&
            b.error().rule == "allow.modulate",
        "regional path prohibition was not enforced");
}
