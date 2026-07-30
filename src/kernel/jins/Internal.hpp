#pragma once

#include "mq/kernel/jins/Catalog.hpp"

namespace mq::kernel::jins::detail {

[[nodiscard]] bool complete(const Identity& identity);

[[nodiscard]] bool valid(motion::Direction direction);

[[nodiscard]] std::expected<void, std::string> validate(
    const Descriptor& descriptor);

} // namespace mq::kernel::jins::detail
