#include "../Test.hpp"

#include "mq/kernel/pitch/System.hpp"
#include "mq/kernel/pitch/order/Compare.hpp"

#include <utility>

void test::witness() {
    using namespace mq::kernel;
    namespace pf = pitch::feasibility;

    const Identity x{"test.witness", "x", "1"};
    const Identity y{"test.witness", "y", "1"};
    const auto zero = pitch::Expression{};
    const auto fourth = pitch::Expression::ratio(4, 3);
    const auto identity = [](std::string name) {
        return Identity{
            "test.witness.constraint",
            std::move(name),
            "1",
        };
    };

    pitch::System affine;
    affine.declare(x);
    affine.declare(y);
    affine.equate({
        identity("sum"),
        {{x, Rational(1)}, {y, Rational(1)}},
        fourth,
        "underdetermined sum",
    });
    affine.bound({
        identity("x"),
        {{x, Rational(-1)}},
        zero,
        "x nonnegative",
    });
    affine.bound({
        identity("y"),
        {{y, Rational(-1)}},
        zero,
        "y nonnegative",
    });
    const auto affineSolution = affine.solve();
    require(
        affineSolution &&
            affineSolution->values.at(x) +
                    affineSolution->values.at(y) ==
                fourth,
        "reverse substitution did not preserve an affine equality");
    const auto xOrder =
        pitch::order::compare(zero, affineSolution->values.at(x));
    const auto yOrder =
        pitch::order::compare(zero, affineSolution->values.at(y));
    require(
        xOrder && xOrder->relation != pitch::order::Relation::Greater &&
            yOrder && yOrder->relation != pitch::order::Relation::Greater,
        "reverse substitution violated a closed lower bound");

    pitch::System box;
    box.declare(x);
    box.bound({
        identity("floor"),
        {{x, Rational(-1)}},
        pitch::Expression::cents(Rational(-1)),
        "one cent floor",
    });
    box.bound({
        identity("ceiling"),
        {{x, Rational(1)}},
        pitch::Expression::cents(Rational(2)),
        "two cent ceiling",
    });
    const auto boxSolution = box.solve();
    require(
        boxSolution &&
            boxSolution->values.at(x) ==
                pitch::Expression::cents(Rational(1)),
        "witness did not choose the exact strongest lower bound");

    pitch::System free;
    free.declare(x);
    free.declare(y);
    const auto freeSolution = free.solve();
    require(
        freeSolution &&
            freeSolution->values.at(x) == zero &&
            freeSolution->values.at(y) == zero,
        "unbounded pitch variables did not receive a lawful exact witness");

    pitch::System retained;
    retained.declare(x);
    retained.declare(y);
    retained.bound({
        identity("retained"),
        {{x, Rational(1)}, {y, Rational(-1)}},
        zero,
        "retained row",
    });
    const auto limited = retained.feasible(pf::Limits{
        10,
        {},
        2,
    });
    require(
        !limited && limited.error().code == pf::Error::Code::Stages,
        "retained projection stages exceeded their budget silently");
}
