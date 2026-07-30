#pragma once

#include "mq/kernel/pitch/Expression.hpp"
#include "mq/kernel/pitch/order/Proof.hpp"

#include <expected>

namespace mq::kernel::pitch::order {

[[nodiscard]] std::expected<Proof, Error> compare(
    const Expression& left,
    const Expression& right,
    Limits limits = {});

[[nodiscard]] std::expected<bool, Error> verify(
    const Proof& proof,
    Limits limits = {});

} // namespace mq::kernel::pitch::order
