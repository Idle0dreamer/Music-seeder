#pragma once

#include "mq/kernel/sayr/Plan.hpp"

namespace mq::kernel::sayr::detail {

[[nodiscard]] bool complete(const Identity& identity);

[[nodiscard]] std::expected<void, std::string> check(
    const Need& need);

[[nodiscard]] std::expected<void, std::string> acyclic(
    const std::map<Identity, Obligation>& obligations);

[[nodiscard]] bool valid(
    const Obligation& obligation,
    const Completion& completion);

} // namespace mq::kernel::sayr::detail
