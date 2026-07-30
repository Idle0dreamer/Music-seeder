#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"

namespace {

namespace kg = mq::kernel::grammar;

mq::kernel::Identity id(const std::string& name) {
    return {"test.grammar", name, "1"};
}

} // namespace

void test::grammar::laws() {
    using namespace mq::kernel;
    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const kg::Evaluator evaluator(fixture.profile.shared);

    const auto anchor = kg::Term::atom(
        id("anchor"),
        operation::Anchor{fixture.center.root});
    const auto enter = kg::Term::atom(
        id("enter"),
        operation::Enter{fixture.jins.root});
    const auto emphasis = kg::Term::atom(
        id("emphasis"),
        operation::Emphasize{fixture.role.root, Rational(1)});
    const auto identity = kg::Term::id(id("id"));

    const auto left = evaluator.derive(
        kg::Term::seq(id("left"), identity, anchor));
    const auto right = evaluator.derive(
        kg::Term::seq(id("right"), anchor, identity));
    require(
        left.outcomes.size() == 1 &&
            right.outcomes.size() == 1 &&
            left.outcomes.front().state == right.outcomes.front().state,
        "Id was not a two-sided identity");

    const auto ab = kg::Term::seq(id("ab"), anchor, enter);
    const auto bc = kg::Term::seq(id("bc"), enter, emphasis);
    const struct {
        kg::Result left;
        kg::Result right;
    } associativity{
        evaluator.derive(
            kg::Term::seq(id("assoc-left"), ab, emphasis)),
        evaluator.derive(
            kg::Term::seq(id("assoc-right"), anchor, bc)),
    };
    require(
        associativity.left.outcomes.size() == 1 &&
            associativity.right.outcomes.size() == 1 &&
            associativity.left.outcomes.front().state ==
                associativity.right.outcomes.front().state,
        "Seq associativity failed");

    const auto failure = kg::Term::seq(
        id("annihilation"),
        kg::Term::fail(id("fail"), "intentional"),
        anchor);
    const auto rejected = evaluator.derive(failure);
    require(
        rejected.outcomes.empty() && rejected.diagnostics.size() == 1,
        "Fail did not left-annihilate Seq");

    const auto repeated = kg::Term::repeat(
        id("repeat"),
        {0, 2},
        emphasis);
    require(repeated.has_value(), repeated.error_or("Repeat rejected bounds"));
    const auto repetitions = evaluator.derive(*repeated);
    require(
        repetitions.outcomes.size() == 3,
        "Repeat did not preserve every bounded count");
    require(
        !kg::Term::repeat(id("bad-repeat"), {2, 1}, emphasis),
        "Repeat accepted reversed bounds");
}
