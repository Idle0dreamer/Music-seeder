#include "Run.hpp"

namespace mq::kernel::grammar::detail {

Batch Runner::scope(
    const Term& unused,
    const Scope& form,
    Frame frame) const {
    static_cast<void>(unused);
    const auto state = frame.outcome.state;
    const auto path = frame.scope;
    const auto bindings = frame.bindings;
    frame.scope.push_back(form.policy.identity);

    auto result = run(form.body, std::move(frame));
    for (auto& output : result.frames) {
        if (!scope::has(form.policy.exports, scope::Part::Center)) {
            output.outcome.state.center = state.center;
        }
        if (!scope::has(form.policy.exports, scope::Part::Jins)) {
            output.outcome.state.jins = state.jins;
        }
        if (!scope::has(form.policy.exports, scope::Part::Tonicization)) {
            output.outcome.state.tonicization = state.tonicization;
        }
        if (!scope::has(form.policy.exports, scope::Part::Evidence)) {
            output.outcome.state.evidence = state.evidence;
        }
        if (!scope::has(form.policy.exports, scope::Part::Cell)) {
            output.outcome.state.cell = state.cell;
        }
        if (!scope::has(form.policy.exports, scope::Part::Path)) {
            output.outcome.state.path = state.path;
        }
        output.scope = path;
        output.bindings = bindings;
    }
    return result;
}

} // namespace mq::kernel::grammar::detail
