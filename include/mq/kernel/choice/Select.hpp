#pragma once

#include "mq/kernel/Identity.hpp"

#include <array>
#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <vector>

namespace mq::kernel::choice {

struct Cost {
    std::array<std::int64_t, 4> tiers{};

    auto operator<=>(const Cost&) const = default;
};

struct Candidate {
    Identity identity;
    Cost cost;
    bool eligible{true};
};

struct Draw {
    Identity expression;
    std::vector<Identity> scope;
    Identity alternative;

    auto operator<=>(const Draw&) const = default;
};

[[nodiscard]] std::expected<Cost, std::string> sum(
    const Cost& left,
    const Cost& right);

[[nodiscard]] std::uint64_t key(
    std::uint64_t seed,
    std::span<const Draw> draws);

[[nodiscard]] std::expected<Identity, std::string> select(
    std::uint64_t seed,
    const Identity& expression,
    std::span<const Identity> scope,
    std::span<const Candidate> candidates,
    const Cost& band = Cost{});

} // namespace mq::kernel::choice
