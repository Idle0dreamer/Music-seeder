#pragma once

#include "mq/kernel/fixture/Set.hpp"

namespace mq::kernel::fixture::detail {

[[nodiscard]] std::expected<Profiles, std::string> profiles(
    const Set& fixture);

} // namespace mq::kernel::fixture::detail
