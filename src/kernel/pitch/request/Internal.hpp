#pragma once

#include "mq/kernel/pitch/request/Run.hpp"

namespace mq::kernel::pitch::request::detail {

[[nodiscard]] std::expected<const performance::Event*, Error> check(
    const state::Snapshot& state,
    const performance::Plan& prefix,
    std::size_t limit);

[[nodiscard]] std::expected<std::optional<order::Proof>, Error> prove(
    const performance::Event& event,
    const Expression& center,
    const performance::Plan& prefix,
    order::Limits limits);

} // namespace mq::kernel::pitch::request::detail
