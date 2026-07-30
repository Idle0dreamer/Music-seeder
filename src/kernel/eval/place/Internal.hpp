#pragma once

#include "mq/kernel/eval/Evaluator.hpp"

namespace mq::kernel::eval::place {

[[nodiscard]] Violation reject(
    std::size_t index,
    std::string rule,
    std::string message);

[[nodiscard]] std::expected<void, Violation> check(
    const operation::Place& event,
    const mq::kernel::jins::Descriptor& descriptor,
    const profile::Set& profile,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] std::expected<
    std::optional<performance::Gesture>,
    Violation> match(
        const operation::Place& event,
        const mq::kernel::jins::Descriptor& descriptor,
        state::Snapshot& state,
        std::size_t index);

} // namespace mq::kernel::eval::place
