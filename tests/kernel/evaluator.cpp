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
        eval::Evaluator(fixture.profile.shared, fixture.path.graph),
        {
            eval::Evaluator(
                fixture.profile.regional.a,
                fixture.path.graph),
            eval::Evaluator(
                fixture.profile.regional.b,
                fixture.path.graph),
        },
    };

    const std::vector<operation::Any> premature{
        operation::Anchor{fixture.center.root},
        operation::Enter{fixture.jins.root},
        operation::Tonicize{
            fixture.jins.branch,
            tonicization::Level::Internal,
        },
    };
    const auto rejected = evaluator.shared.run({}, premature);
    require(!rejected, "one entered jins incorrectly proved tonicization");
    require(
        rejected.error().rule == "threshold.internal.dwell",
        "tonicization failure did not identify missing evidence");

    const std::vector<operation::Any> established{
        operation::Anchor{fixture.center.root},
        operation::Enter{fixture.jins.root},
        operation::Emphasize{fixture.role.ghammaz, Rational(2)},
        operation::Dwell{fixture.role.ghammaz, Rational(2)},
        operation::Emit{fixture.cell},
        operation::Cadence{fixture.cadence, Rational(1)},
        operation::Tonicize{
            fixture.jins.branch,
            tonicization::Level::Internal,
        },
        operation::Modulate{
            fixture.path.direct,
            fixture.center.branch,
            tonicization::Level::Internal,
        },
        operation::Return{fixture.center.root},
    };
    const auto accepted = evaluator.shared.run({}, established);
    require(accepted.has_value(), "legal neutral program was rejected");
    require(
        accepted->center.stack.size() == 1 &&
            accepted->center.stack.back() == fixture.center.root,
        "return did not restore the established center");

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
