#pragma once

#include "mq/kernel/pitch/tendency/sequence/Solve.hpp"

namespace mq::kernel::pitch::tendency::sequence::detail {

[[nodiscard]] std::expected<void, Error> validate(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Tier> tiers,
    std::size_t limit);

[[nodiscard]] Equation freeze(
    const Tier& tier,
    const Target& target,
    const Expression& prediction);

} // namespace mq::kernel::pitch::tendency::sequence::detail
