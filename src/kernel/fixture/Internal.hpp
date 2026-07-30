#pragma once

#include "mq/kernel/fixture/Set.hpp"

namespace mq::kernel::fixture::detail {

[[nodiscard]] std::expected<Profiles, std::string> profiles(
    const Set& fixture);

[[nodiscard]] std::expected<jins::Catalog, std::string> catalog(
    const Set& fixture);

[[nodiscard]] std::expected<mq::kernel::sayr::Plan, std::string> sayr(
    const Set& fixture);

} // namespace mq::kernel::fixture::detail
