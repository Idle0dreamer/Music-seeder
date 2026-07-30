#pragma once

namespace mq::kernel::tonicization {

enum class Level {
    Color,
    Internal,
    Maqam,
};

[[nodiscard]] const char* name(Level value) noexcept;

} // namespace mq::kernel::tonicization
