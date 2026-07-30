#include "mq/kernel/eval/Evaluator.hpp"

namespace mq::kernel::eval {

Evaluator::Evaluator(const profile::Set& profile)
    : profile_(profile) {}

Evaluator::Evaluator(const profile::Set& profile, const path::Graph& paths)
    : profile_(profile),
      paths_(&paths) {}

std::expected<state::Snapshot, Violation> Evaluator::run(
    state::Snapshot state,
    std::span<const operation::Any> program) const {
    const auto offset = state.trace.events.size();
    for (std::size_t index = 0; index < program.size(); ++index) {
        auto next = apply(std::move(state), program[index], offset + index);
        if (!next) {
            return std::unexpected(next.error());
        }
        state = std::move(*next);
    }
    return state;
}

} // namespace mq::kernel::eval
