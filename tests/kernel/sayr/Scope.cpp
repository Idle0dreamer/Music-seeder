#include "../Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.sayr.scope", std::move(name), "1"};
}

} // namespace

void test::sayr::scope() {
    using namespace mq::kernel;
    namespace kg = mq::kernel::grammar;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const auto generated = fixture::generation::make(fixture);
    require(generated.has_value(), generated.error_or("generation failed"));
    const eval::Context context{
        .jins = {&fixture.catalog},
        .path = {&fixture.path.graph},
        .sayr = {&fixture.sayr.plan},
        .grammar = {},
    };
    auto actions = generated->program.stay.stages.front().actions;
    actions.resize(actions.size() - 2);
    const auto prepared =
        eval::Evaluator(fixture.profile.shared, context).run({}, actions);
    require(prepared.has_value(), prepared ? "" : prepared.error().message);

    const kg::Evaluator evaluator(fixture.profile.shared, context);
    const auto fulfill = kg::Term::atom(
        id("fulfill"),
        operation::sayr::Fulfill{
            sort::ObligationId{fixture.sayr.obligation.establish},
        });
    const auto hidden = evaluator.derive(
        kg::Term::scope(
            id("hidden"),
            {id("policy.hidden"), kg::scope::Part::None},
            fulfill),
        *prepared);
    require(
        hidden.outcomes.size() == 1 &&
            hidden.outcomes.front().state.sayr.completed.empty() &&
            hidden.outcomes.front().state.trace.events.size() ==
                prepared->trace.events.size() + 1,
        "non-exported sayr progress leaked or erased trace evidence");

    const auto visible = evaluator.derive(
        kg::Term::scope(
            id("visible"),
            {id("policy.visible"), kg::scope::Part::Sayr},
            fulfill),
        *prepared);
    require(
        visible.outcomes.size() == 1 &&
            visible.outcomes.front().state.sayr.completed.contains(
                sort::ObligationId{fixture.sayr.obligation.establish}),
        "explicit sayr scope export lost completion state");
}
