#pragma once

namespace mq::kernel::motion {

enum class Direction {
    Start,
    Same,
    Rise,
    Fall,
};

[[nodiscard]] const char* name(Direction direction) noexcept;

} // namespace mq::kernel::motion
