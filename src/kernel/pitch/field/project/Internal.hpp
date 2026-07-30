#pragma once

#include "mq/kernel/pitch/field/project/Run.hpp"

#include <set>

namespace mq::kernel::pitch::field::project::detail {

[[nodiscard]] bool complete(const Identity& identity);

[[nodiscard]] const Identity& key(const Source& source);

[[nodiscard]] std::expected<void, Error> check(
    const Plan& plan,
    Limits limits);

[[nodiscard]] std::expected<std::optional<Fact>, Error> read(
    const Source& source,
    const state::Snapshot& state);

} // namespace mq::kernel::pitch::field::project::detail
