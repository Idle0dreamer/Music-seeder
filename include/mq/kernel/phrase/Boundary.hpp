#pragma once

namespace mq::kernel::phrase {

enum class Boundary {
    Open,
    Closed,
};

[[nodiscard]] const char* name(Boundary boundary) noexcept;

} // namespace mq::kernel::phrase
