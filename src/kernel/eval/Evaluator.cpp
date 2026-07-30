#include "mq/kernel/eval/Evaluator.hpp"

namespace mq::kernel::eval {

Evaluator::Evaluator(const profile::Set& profile)
    : Evaluator(profile, Context{}) {}

Evaluator::Evaluator(
    const profile::Set& profile,
    Context context)
    : profile_(profile),
      context_(context) {}

Evaluator::Evaluator(const profile::Set& profile, const path::Graph& paths)
    : Evaluator(
          profile,
          Context{
              .jins = {},
              .path = {&paths},
              .sayr = {},
          }) {}

Evaluator::Evaluator(
    const profile::Set& profile,
    const jins::Catalog& catalog)
    : Evaluator(
          profile,
          Context{
              .jins = {&catalog},
              .path = {},
              .sayr = {},
          }) {}

Evaluator::Evaluator(
    const profile::Set& profile,
    const jins::Catalog& catalog,
    const path::Graph& paths)
    : Evaluator(
          profile,
          Context{
              .jins = {&catalog},
              .path = {&paths},
              .sayr = {},
          }) {}

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
