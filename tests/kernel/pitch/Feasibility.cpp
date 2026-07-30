#include "../Test.hpp"

#include "mq/kernel/pitch/System.hpp"

#include <utility>

void test::feasibility_limits() {
    using namespace mq::kernel;
    namespace pf = pitch::feasibility;

    const Identity x{"test.feasibility.limit", "x", "1"};
    const Identity y{"test.feasibility.limit", "y", "1"};
    const Identity unknown{"test.feasibility.limit", "unknown", "1"};
    const auto id = [](std::string name) {
        return Identity{
            "test.feasibility.limit.rule",
            std::move(name),
            "1",
        };
    };

    pitch::System absent;
    absent.declare(x);
    absent.bound({
        id("absent"),
        {{unknown, Rational(1)}},
        {},
        "unknown role",
    });
    const auto missing = absent.feasible();
    require(
        !missing && missing.error().code == pf::Error::Code::Input,
        "unknown inequality variable was accepted");

    pitch::System rows;
    rows.declare(x);
    rows.declare(y);
    rows.bound({
        id("xy"),
        {{x, Rational(1)}, {y, Rational(-1)}},
        {},
        "x at or below y",
    });
    rows.bound({
        id("y"),
        {{y, Rational(1)}},
        {},
        "y at or below zero",
    });
    rows.bound({
        id("x"),
        {{x, Rational(-1)}},
        pitch::Expression::cents(Rational(-1)),
        "x at or above one",
    });
    const auto limited = rows.feasible(pf::Limits{
        2,
        {},
    });
    require(
        !limited && limited.error().code == pf::Error::Code::Rows,
        "Fourier-Motzkin row budget did not fail explicitly");

    pitch::System proof;
    proof.bound({
        id("proof"),
        {},
        pitch::Expression::ratio(3, 2) * Rational(12),
        "large exact constant",
    });
    const auto exhausted = proof.feasible(pf::Limits{
        10,
        pitch::order::Limits{10, 1024},
    });
    require(
        !exhausted &&
            exhausted.error().code == pf::Error::Code::Proof &&
            exhausted.error().proof &&
            exhausted.error().proof->code ==
                pitch::order::Error::Code::Exponent,
        "inequality proof budget silently approximated a comparison");
}
