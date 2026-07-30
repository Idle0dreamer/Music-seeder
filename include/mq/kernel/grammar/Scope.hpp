#pragma once

#include "mq/kernel/Identity.hpp"

#include <cstdint>

namespace mq::kernel::grammar::scope {

enum class Part : std::uint16_t {
    None = 0,
    Center = 1U << 0U,
    Jins = 1U << 1U,
    Tonicization = 1U << 2U,
    Evidence = 1U << 3U,
    Cell = 1U << 4U,
    Path = 1U << 5U,
    Melody = 1U << 6U,
    Phrase = 1U << 7U,
    Output = 1U << 8U,
    Gesture = 1U << 9U,
    Sayr = 1U << 10U,
};

constexpr Part operator|(Part left, Part right) noexcept {
    return static_cast<Part>(
        static_cast<std::uint16_t>(left) |
        static_cast<std::uint16_t>(right));
}

[[nodiscard]] constexpr bool has(Part set, Part part) noexcept {
    return (
        static_cast<std::uint16_t>(set) &
        static_cast<std::uint16_t>(part)) != 0;
}

struct Policy {
    Identity identity;
    Part exports{Part::None};
};

} // namespace mq::kernel::grammar::scope
