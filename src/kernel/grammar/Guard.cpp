#include "mq/kernel/grammar/Guard.hpp"

#include <type_traits>

namespace mq::kernel::grammar::guard {

bool holds(
    const Predicate& predicate,
    const state::Snapshot& state,
    const std::set<Identity>& bindings) {
    return std::visit(
        [&](const auto& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, Bound>) {
                return bindings.contains(value.binding);
            } else if constexpr (std::is_same_v<T, Center>) {
                return !state.center.stack.empty() &&
                       state.center.stack.back().identity == value.identity;
            } else if constexpr (std::is_same_v<T, Jins>) {
                return state.jins.active && state.jins.active->identity == value.identity;
            } else {
                const auto found = state.evidence.amount.find(value.kind);
                const auto actual = found == state.evidence.amount.end()
                                      ? Rational(0)
                                      : found->second;
                return actual >= value.minimum;
            }
        },
        predicate);
}

} // namespace mq::kernel::grammar::guard
