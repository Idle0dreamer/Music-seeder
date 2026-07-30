#include "../../../Test.hpp"

#include "mq/kernel/pitch/tendency/sequence/Solve.hpp"

#include <algorithm>
#include <utility>

void test::sequence::laws() {
    using namespace mq::kernel;
    namespace ps = pitch::tendency::sequence;
    namespace pt = pitch::tendency;

    const Identity x{"test.sequence.law", "x", "1"};
    const auto id = [](std::string name) {
        return Identity{"test.sequence.law.rule", std::move(name), "1"};
    };
    const std::vector<Identity> variables{x};
    const std::vector<pitch::Equation> equations;
    const std::vector<pitch::Inequality> inequalities;

    const pt::Tier zero{
        id("tier.zero"),
        {{id("zero"), {{x, Rational(1)}}, {}, Rational(1), "zero"}},
    };
    const pt::Tier ten{
        id("tier.ten"),
        {{id("ten"), {{x, Rational(1)}},
          pitch::Expression::cents(Rational(10)), Rational(1), "ten"}},
    };
    const std::vector<pt::Tier> forward{zero, ten};
    const std::vector<pt::Tier> reversed{ten, zero};
    const auto first = ps::solve(
        variables,
        equations,
        inequalities,
        forward);
    const auto second = ps::solve(
        variables,
        equations,
        inequalities,
        reversed);
    require(
        first && second &&
            first->solution.values.at(x) == pitch::Expression{} &&
            second->solution.values.at(x) ==
                pitch::Expression::cents(Rational(10)),
        "semantic tier order was normalized away");

    const auto limited = ps::solve(
        variables,
        equations,
        inequalities,
        forward,
        ps::Limits{1, {}});
    require(
        !limited && limited.error().code == ps::Error::Code::Count,
        "tier-count budget exhaustion did not fail explicitly");

    auto shuffled = zero;
    shuffled.targets.push_back({
        id("five"),
        {{x, Rational(1)}},
        pitch::Expression::cents(Rational(5)),
        Rational(1),
        "five",
    });
    auto reordered = shuffled;
    std::ranges::reverse(reordered.targets);
    const std::vector<pt::Tier> left{shuffled};
    const std::vector<pt::Tier> right{reordered};
    const auto normal = ps::solve(
        variables,
        equations,
        inequalities,
        left);
    const auto stored = ps::solve(
        variables,
        equations,
        inequalities,
        right);
    require(
        normal && stored &&
            normal->solution.values == stored->solution.values &&
            normal->steps.front().predictions ==
                stored->steps.front().predictions,
        "within-tier target storage order changed the sequence");

    auto duplicate = ten;
    duplicate.targets.front().identity = zero.targets.front().identity;
    const std::vector<pt::Tier> repeated{zero, duplicate};
    const auto identity = ps::solve(
        variables,
        equations,
        inequalities,
        repeated);
    require(
        !identity && identity.error().code == ps::Error::Code::Input,
        "cross-tier target identity collision was accepted");
}
