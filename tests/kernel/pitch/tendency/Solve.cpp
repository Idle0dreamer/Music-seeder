#include "../../Test.hpp"

#include "mq/kernel/pitch/tendency/Solve.hpp"

#include <utility>

void test::tendency::solve() {
    using namespace mq::kernel;
    namespace pt = pitch::tendency;

    const Identity x{"test.tendency", "x", "1"};
    const Identity y{"test.tendency", "y", "1"};
    const auto id = [](std::string name) {
        return Identity{"test.tendency.rule", std::move(name), "1"};
    };

    const std::vector<Identity> one{x};
    const std::vector<pitch::Equation> equations;
    const std::vector<pitch::Inequality> none;
    const auto fourth = pitch::Expression::ratio(4, 3);
    const std::vector<pt::Target> symbolic{{
        id("symbolic"),
        {{x, Rational(1)}},
        fourth,
        Rational(1),
        "symbolic tendency",
    }};
    const auto exact = pt::solve(one, equations, none, symbolic);
    require(
        exact &&
            exact->solution.values.at(x) == fourth &&
            exact->predictions.at(id("symbolic")) == fourth &&
            exact->active.empty(),
        "unconstrained symbolic tendency did not reach its exact target");

    const std::vector<pt::Target> weighted{
        {
            id("low"),
            {{x, Rational(1)}},
            {},
            Rational(1),
            "low tendency",
        },
        {
            id("high"),
            {{x, Rational(1)}},
            pitch::Expression::cents(Rational(10)),
            Rational(3),
            "high tendency",
        },
    };
    const auto center = pt::solve(one, equations, none, weighted);
    require(
        center &&
            center->solution.values.at(x) ==
                pitch::Expression::cents(Rational(15, 2)),
        "positive rational weights did not produce the exact center");

    const Identity ceiling = id("ceiling");
    const std::vector<pitch::Inequality> upper{{
        ceiling,
        {{x, Rational(1)}},
        pitch::Expression::cents(Rational(5)),
        "five cent ceiling",
    }};
    const std::vector<pt::Target> ten{{
        id("ten"),
        {{x, Rational(1)}},
        pitch::Expression::cents(Rational(10)),
        Rational(1),
        "ten cent tendency",
    }};
    const auto capped = pt::solve(one, equations, upper, ten);
    require(
        capped &&
            capped->solution.values.at(x) ==
                pitch::Expression::cents(Rational(5)) &&
            capped->active == std::vector<Identity>{ceiling},
        "active upper bound did not constrain the exact optimum");

    const Identity floor = id("floor");
    const std::vector<pitch::Inequality> lower{{
        floor,
        {{x, Rational(-1)}},
        pitch::Expression::cents(Rational(-5)),
        "five cent floor",
    }};
    const std::vector<pt::Target> zero{{
        id("zero"),
        {{x, Rational(1)}},
        {},
        Rational(1),
        "zero tendency",
    }};
    const auto raised = pt::solve(one, equations, lower, zero);
    require(
        raised &&
            raised->solution.values.at(x) ==
                pitch::Expression::cents(Rational(5)) &&
            raised->active == std::vector<Identity>{floor},
        "active lower bound used the wrong KKT multiplier sign");

    const std::vector<Identity> two{x, y};
    const Identity sum = id("sum");
    const std::vector<pitch::Inequality> coupled{{
        sum,
        {{x, Rational(1)}, {y, Rational(1)}},
        pitch::Expression::cents(Rational(10)),
        "coupled ceiling",
    }};
    const std::vector<pt::Target> pair{
        {id("x"), {{x, Rational(1)}},
         pitch::Expression::cents(Rational(10)), Rational(1), "x"},
        {id("y"), {{y, Rational(1)}},
         pitch::Expression::cents(Rational(10)), Rational(1), "y"},
    };
    const auto corner = pt::solve(two, equations, coupled, pair);
    require(
        corner &&
            corner->solution.values.at(x) ==
                pitch::Expression::cents(Rational(5)) &&
            corner->solution.values.at(y) ==
                pitch::Expression::cents(Rational(5)) &&
            corner->active == std::vector<Identity>{sum},
        "coupled active constraint did not produce the exact KKT point");
}
