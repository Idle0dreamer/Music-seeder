#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"

namespace {

namespace kg = mq::kernel::grammar;

mq::kernel::Identity id(const std::string& name) {
    return {"test.scope", name, "1"};
}

} // namespace

void test::grammar::relation() {
    using namespace mq::kernel;
    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const kg::Evaluator evaluator(
        fixture.profile.shared,
        fixture.catalog);

    const auto anchor = kg::Term::atom(
        id("anchor"),
        operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}});
    const auto enter = kg::Term::atom(
        id("enter"),
        operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}});
    const auto denied = kg::Term::atom(
        id("denied"),
        operation::Anchor{mq::kernel::sort::CenterId{fixture.center.branch}});

    const auto alternatives = kg::Term::alt(
        id("alt"),
        {
            {id("a"), {}, anchor},
            {id("b"), {}, enter},
            {id("illegal"), {}, denied},
        });
    require(
        alternatives.has_value(),
        alternatives.error_or("Alt construction failed"));
    const auto result = evaluator.derive(*alternatives);
    require(
        result.outcomes.size() == 2 && result.diagnostics.size() == 1,
        "Alt did not retain complete legal and illegal branch results");

    const auto reordered = kg::Term::alt(
        id("alt"),
        {
            {id("illegal"), {}, denied},
            {id("b"), {}, enter},
            {id("a"), {}, anchor},
        });
    require(reordered.has_value(), reordered.error_or("Alt reorder failed"));
    require(
        evaluator.choose(91, *alternatives)->state ==
            evaluator.choose(91, *reordered)->state,
        "Alt storage order changed seeded resolution");

    const auto binding = id("binding");
    const auto guarded = kg::Term::guard(
        id("guard"),
        kg::guard::Bound{binding},
        anchor);
    const auto bound =
        kg::Term::bind(id("bind"), binding, guarded);
    require(
        evaluator.derive(bound).outcomes.size() == 1,
        "Bind was unavailable inside its lexical body");
    const auto leaked = kg::Term::seq(
        id("leak"),
        bound,
        kg::Term::guard(
            id("outside"),
            kg::guard::Bound{binding},
            kg::Term::id(id("id"))));
    require(
        evaluator.derive(leaked).outcomes.empty(),
        "Bind leaked into the following lexical scope");

    const auto body = kg::Term::seq(
        id("body"),
        anchor,
        kg::Term::seq(id("body-tail"), enter, guarded));
    const auto local = kg::Term::scope(
        id("scope"),
        {id("phrase"), kg::scope::Part::None},
        kg::Term::bind(id("inner-bind"), binding, body));
    const auto scoped = evaluator.derive(local);
    require(
        scoped.outcomes.size() == 1,
        "Scope rejected a legal bound body");
    const auto& state = scoped.outcomes.front().state;
    require(
        state.center.stack.empty() && !state.jins.active &&
            state.trace.events.size() == 3,
        "Scope failed to restore local musical state");

    const auto decision = kg::Term::alt(
        id("scoped-alt"),
        {{id("only"), {}, anchor}});
    require(decision.has_value(), decision.error_or("scoped Alt failed"));
    const auto nested = kg::Term::scope(
        id("decision-scope"),
        {id("phrase"), kg::scope::Part::Center},
        *decision);
    const auto derived = evaluator.derive(nested);
    require(
        derived.outcomes.front().decisions.front().scope ==
            std::vector<Identity>{id("phrase")},
        "scope path was flattened or lost from choice identity");
}
