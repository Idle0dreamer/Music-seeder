#pragma once

#include "mq/kernel/Identity.hpp"

#include <cstdint>

namespace mq::kernel::grammar::scope {

enum class Part : std::uint8_t {
    None = 0,
    Center = 1U << 0U,
    Jins = 1U << 1U,
    Tonicization = 1U << 2U,
    Evidence = 1U << 3U,
    Cell = 1U << 4U,
    Path = 1U << 5U,
};

constexpr Part operator|(Part left, Part right) noexcept {
    return static_cast<Part>(
        static_cast<std::uint8_t>(left) |
        static_cast<std::uint8_t>(right));
}

[[nodiscard]] constexpr bool has(Part set, Part part) noexcept {
    return (
        static_cast<std::uint8_t>(set) &
        static_cast<std::uint8_t>(part)) != 0;
}

struct Policy {
    Identity identity;
    Part exports{Part::None};
};

} // namespace mq::kernel::grammar::scope
