#pragma once

#include "../Internal.hpp"

namespace mq::kernel::pitch::field::project::detail::phrase {

[[nodiscard]] std::expected<
    const performance::Event*,
    Error> current(const state::Snapshot& state);

[[nodiscard]] const mq::kernel::phrase::Frame* active(
    const state::Snapshot& state);

[[nodiscard]] std::expected<
    const mq::kernel::phrase::Span*,
    Error> completed(const state::Snapshot& state);

[[nodiscard]] std::expected<std::optional<Fact>, Error> absent(
    const Identity& key,
    Presence presence);

} // namespace mq::kernel::pitch::field::project::detail::phrase
