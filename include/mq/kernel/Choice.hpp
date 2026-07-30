#pragma once

#include "mq/kernel/Identity.hpp"

#include <array>
#include <cstdint>
#include <expected>
#include <span>
#include <string>

namespace mq::kernel {

struct Cost {
    std::array<std::int64_t, 4> tiers{};

    auto operator<=>(const Cost&) const = default;
};

struct Candidate {
    Identity identity;
    Cost cost;
    bool eligible{true};
};

[[nodiscard]] std::expected<Identity, std::string> select(
    std::uint64_t seed,
    const Identity& expression,
    const std::string& scopePath,
    std::span<const Candidate> candidates);

} // namespace mq::kernel

