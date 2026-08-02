#pragma once

#include "mq/kernel/maqam/Package.hpp"

#include <expected>
#include <string>
#include <string_view>

namespace mq::kernel::maqam::collection {

[[nodiscard]] std::expected<Scaffold, std::string> build(
    std::string_view package);

} // namespace mq::kernel::maqam::collection
