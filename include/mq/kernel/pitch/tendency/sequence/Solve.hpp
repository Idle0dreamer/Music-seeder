#pragma once

#include "mq/kernel/pitch/tendency/Solve.hpp"
#include "mq/kernel/pitch/tendency/Tier.hpp"

namespace mq::kernel::pitch::tendency::sequence {

struct Limits {
    std::size_t count{64};
    tendency::Limits tier{};
};

struct Error {
    enum class Code {
        Input,
        Count,
        Solve,
        Internal,
    };

    Code code;
    std::string message;
    std::optional<tendency::Error> cause;
};

struct Step {
    Identity tier;
    std::vector<Identity> active;
    std::map<Identity, Expression> predictions;
};

struct Result {
    Solution solution;
    std::vector<Step> steps;
};

[[nodiscard]] std::expected<Result, Error> solve(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Tier> tiers,
    Limits limits = {});

} // namespace mq::kernel::pitch::tendency::sequence
