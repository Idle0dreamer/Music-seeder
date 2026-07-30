#include "../../Test.hpp"

#include "mq/kernel/pitch/tendency/Solve.hpp"

#include <algorithm>
#include <utility>

void test::tendency::laws() {
    using namespace mq::kernel;
    namespace pt = pitch::tendency;

    const Identity x{"test.tendency.law", "x", "1"};
    const auto id = [](std::string name) {
        return Identity{"test.tendency.law.rule", std::move(name), "1"};
    };
    const std::vector<Identity> variables{x};
    const std::vector<pitch::Equation> equations;
    std::vector<pitch::Inequality> bounds{
        {
            id("floor"),
            {{x, Rational(-1)}},
            pitch::Expression::cents(Rational(-2)),
            "floor",
        },
        {
            id("ceiling"),
            {{x, Rational(1)}},
            pitch::Expression::cents(Rational(5)),
            "ceiling",
        },
    };
    std::vector<pt::Target> targets{
        {id("low"), {{x, Rational(1)}}, {}, Rational(1), "low"},
        {id("high"), {{x, Rational(1)}},
         pitch::Expression::cents(Rational(10)), Rational(3), "high"},
    };

    const auto forward = pt::solve(
        variables,
        equations,
        bounds,
        targets);
    std::ranges::reverse(bounds);
    std::ranges::reverse(targets);
    const auto reversed = pt::solve(
        variables,
        equations,
        bounds,
        targets);
    require(
        forward && reversed &&
            forward->solution.values == reversed->solution.values &&
            forward->active == reversed->active &&
            forward->predictions == reversed->predictions,
        "tendency result changed under input storage reordering");

    const auto limited = pt::solve(
        variables,
        equations,
        bounds,
        targets,
        pt::Limits{1, {}});
    require(
        !limited && limited.error().code == pt::Error::Code::Sets,
        "active-set budget exhaustion did not fail explicitly");

    const std::vector<pt::Target> invalid{{
        id("invalid"),
        {{x, Rational(1)}},
        {},
        Rational(0),
        "zero weight",
    }};
    const auto weight = pt::solve(
        variables,
        equations,
        bounds,
        invalid);
    require(
        !weight && weight.error().code == pt::Error::Code::Input,
        "non-positive tendency weight was accepted");

    const Identity repeated = id("repeated");
    const std::vector<pt::Target> duplicate{
        {repeated, {{x, Rational(1)}}, {}, Rational(1), "first"},
        {repeated, {{x, Rational(1)}}, {}, Rational(1), "second"},
    };
    const auto identity = pt::solve(
        variables,
        equations,
        bounds,
        duplicate);
    require(
        !identity && identity.error().code == pt::Error::Code::Input,
        "duplicate tendency identity was accepted");

    const std::vector<pitch::Inequality> impossible{
        {id("at.most.zero"), {{x, Rational(1)}}, {}, "ceiling"},
        {id("at.least.one"), {{x, Rational(-1)}},
         pitch::Expression::cents(Rational(-1)), "floor"},
    };
    const auto infeasible = pt::solve(
        variables,
        equations,
        impossible,
        targets);
    require(
        !infeasible &&
            infeasible.error().code == pt::Error::Code::Infeasible &&
            infeasible.error().constraints.size() == 2,
        "infeasible hard region was not distinguished from KKT failure");
}
