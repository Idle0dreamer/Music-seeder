#pragma once

#include "mq/kernel/math/Natural.hpp"
#include "mq/kernel/pitch/order/Compare.hpp"

#include <map>

namespace mq::kernel::pitch::order::detail {

using Powers = std::map<std::int64_t, Rational>;

[[nodiscard]] bool prime(std::int64_t value);

[[nodiscard]] Powers normalize(
    const Expression& left,
    const Expression& right);

[[nodiscard]] std::expected<std::uint64_t, Error> scale(
    const Powers& powers);

[[nodiscard]] std::expected<math::Natural, Error> product(
    const Powers& powers,
    std::uint64_t scale,
    bool positive,
    Limits limits);

[[nodiscard]] std::expected<Proof, Error> prove(
    Powers powers,
    Limits limits);

} // namespace mq::kernel::pitch::order::detail
