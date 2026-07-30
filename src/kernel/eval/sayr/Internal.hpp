#pragma once

#include "mq/kernel/eval/Evaluator.hpp"

namespace mq::kernel::eval::sayr {

using Proofs = std::vector<mq::kernel::sayr::proof::Item>;
using Result = std::expected<Proofs, Violation>;

[[nodiscard]] Violation reject(
    std::size_t index,
    std::string rule,
    std::string message);

[[nodiscard]] bool consistent(
    const state::Sayr& state);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Jins& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Center& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Role& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Region& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Gesture& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Phrase& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Path& need,
    const state::Snapshot& state,
    std::size_t index);

[[nodiscard]] Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Evidence& need,
    const state::Snapshot& state,
    std::size_t index);

} // namespace mq::kernel::eval::sayr
