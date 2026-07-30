#include "../../../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"
#include "mq/kernel/pitch/field/project/Run.hpp"
#include "mq/kernel/pitch/field/solve/Run.hpp"

#include <algorithm>

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.project.scope", std::move(name), "1"};
}

} // namespace

void test::project::scope() {
    using namespace mq::kernel;
    namespace kg = mq::kernel::grammar;
    namespace pf = mq::kernel::pitch::field;
    namespace pp = pf::project;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const kg::Evaluator evaluator(fixture.profile.shared);

    const auto local = kg::Term::scope(
        id("term.scope"),
        {id("scope"), kg::scope::Part::None},
        kg::Term::atom(
            id("term.enter"),
            operation::Enter{fixture.jins.branch}));
    state::Snapshot initial;
    initial.jins.active = fixture.jins.root;
    const auto grammar = evaluator.derive(local, initial);
    require(
        grammar.outcomes.size() == 1 &&
            grammar.outcomes.front().state.jins.active ==
                fixture.jins.root &&
            !grammar.outcomes.front().state.trace.events.empty(),
        "grammar fixture did not restore scoped jins state");

    const auto key = id("context.jins");
    const auto context = pp::run(
        pp::Plan{{pp::jins::Read{key}}},
        grammar.outcomes.front().state);
    require(
        context.has_value(),
        context ? "" : context.error().message);

    const auto x = id("variable.x");
    const auto tier = id("tier.structural");
    const auto rootRule = id("rule.root");
    const auto branchRule = id("rule.branch");
    const pf::Schema schema{
        {x},
        {tier},
        {
            {
                {{{key, fixture.jins.root}}},
                pf::Aim{
                    tier,
                    {
                        rootRule,
                        {{x, Rational(1)}},
                        {},
                        Rational(1),
                        "neutral root fixture",
                    },
                },
            },
            {
                {{{key, fixture.jins.branch}}},
                pf::Aim{
                    tier,
                    {
                        branchRule,
                        {{x, Rational(1)}},
                        pitch::Expression::cents(Rational(10)),
                        Rational(1),
                        "neutral branch fixture",
                    },
                },
            },
        },
        {{id("need.jins"), {rootRule, branchRule}}},
    };
    const auto solved = pf::solve::run(schema, *context);
    require(
        solved &&
            solved->solution.values.at(x) == pitch::Expression{} &&
            std::ranges::find(solved->rules, rootRule) !=
                solved->rules.end() &&
            std::ranges::find(solved->rules, branchRule) ==
                solved->rules.end(),
        "discarded scoped state leaked into calculated pitch field");
}
