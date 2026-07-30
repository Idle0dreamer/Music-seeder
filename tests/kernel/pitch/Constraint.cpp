#include "../Test.hpp"

#include "mq/kernel/pitch/System.hpp"

void test::constraint() {
    using namespace mq::kernel;
    namespace pf = pitch::feasibility;

    const Identity x{"test.constraint", "x", "1"};
    const Identity shared{"test.constraint.rule", "shared", "1"};

    pitch::System duplicate;
    duplicate.declare(x);
    duplicate.equate({
        shared,
        {{x, Rational(1)}},
        {},
        "anchor",
    });
    duplicate.bound({
        shared,
        {{x, Rational(1)}},
        {},
        "ceiling",
    });
    const auto repeated = duplicate.feasible();
    require(
        !repeated && repeated.error().code == pf::Error::Code::Input,
        "duplicate hard-constraint identity was accepted");

    pitch::System incomplete;
    incomplete.declare(x);
    incomplete.bound({
        {},
        {{x, Rational(1)}},
        {},
        "missing identity",
    });
    const auto missing = incomplete.feasible();
    require(
        !missing && missing.error().code == pf::Error::Code::Input,
        "incomplete hard-constraint identity was accepted");

    const Identity low{"test.constraint.rule", "low", "1"};
    const Identity high{"test.constraint.rule", "high", "1"};
    pitch::System contradiction;
    contradiction.declare(x);
    contradiction.equate({
        low,
        {{x, Rational(1)}},
        {},
        "zero anchor",
    });
    contradiction.equate({
        high,
        {{x, Rational(1)}},
        pitch::Expression::cents(Rational(1)),
        "one cent anchor",
    });
    const auto result = contradiction.feasible();
    require(
        result &&
            result->status == pf::Status::Infeasible &&
            result->constraints.size() == 2 &&
            result->constraints[0] != result->constraints[1],
        "infeasibility trace lost stable constraint identities");
}
