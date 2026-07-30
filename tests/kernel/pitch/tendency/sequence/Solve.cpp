#include "../../../Test.hpp"

#include "mq/kernel/pitch/tendency/sequence/Solve.hpp"

#include <utility>

void test::sequence::solve() {
    using namespace mq::kernel;
    namespace ps = pitch::tendency::sequence;
    namespace pt = pitch::tendency;

    const Identity x{"test.sequence", "x", "1"};
    const Identity y{"test.sequence", "y", "1"};
    const auto id = [](std::string name) {
        return Identity{"test.sequence.rule", std::move(name), "1"};
    };
    const std::vector<Identity> variables{x, y};
    const std::vector<pitch::Equation> equations;
    const std::vector<pitch::Inequality> inequalities;

    const pt::Tier structural{
        id("structural"),
        {{
            id("sum"),
            {{x, Rational(1)}, {y, Rational(1)}},
            pitch::Expression::cents(Rational(10)),
            Rational(1),
            "structural sum",
        }},
    };
    const pt::Tier regional{
        id("regional"),
        {{
            id("x.zero"),
            {{x, Rational(1)}},
            {},
            Rational(1),
            "regional x",
        }},
    };
    const std::vector<pt::Tier> tiers{structural, regional};
    const auto result = ps::solve(
        variables,
        equations,
        inequalities,
        tiers);
    require(
        result &&
            result->solution.values.at(x) == pitch::Expression{} &&
            result->solution.values.at(y) ==
                pitch::Expression::cents(Rational(10)) &&
            result->steps.size() == 2 &&
            result->steps[0].predictions.at(id("sum")) ==
                pitch::Expression::cents(Rational(10)) &&
            result->steps[1].predictions.at(id("x.zero")) ==
                pitch::Expression{},
        "weaker tier did not resolve the stronger tier null space exactly");

    const pt::Tier weakConflict{
        id("surface"),
        {{
            id("x.ten"),
            {{x, Rational(1)}},
            pitch::Expression::cents(Rational(10)),
            Rational(1),
            "conflicting surface target",
        }},
    };
    const std::vector<pt::Tier> constrained{
        regional,
        weakConflict,
    };
    const auto preserved = ps::solve(
        variables,
        equations,
        inequalities,
        constrained);
    require(
        preserved &&
            preserved->solution.values.at(x) == pitch::Expression{} &&
            preserved->steps[0].predictions.at(id("x.zero")) ==
                pitch::Expression{} &&
            preserved->steps[1].predictions.at(id("x.ten")) ==
                pitch::Expression{},
        "weaker conflicting target degraded a stronger prediction");

    const std::vector<pt::Tier> one{regional};
    const auto sequence = ps::solve(
        variables,
        equations,
        inequalities,
        one);
    const auto direct = pt::solve(
        variables,
        equations,
        inequalities,
        regional.targets);
    require(
        sequence && direct &&
            sequence->solution.values == direct->solution.values &&
            sequence->steps.front().predictions == direct->predictions,
        "single-tier sequence disagreed with the KKT tier operator");
}
