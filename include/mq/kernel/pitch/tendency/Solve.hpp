#pragma once

#include "mq/kernel/pitch/Feasibility.hpp"
#include "mq/kernel/pitch/tendency/Target.hpp"

#include <expected>
#include <map>
#include <optional>
#include <span>

namespace mq::kernel::pitch::tendency {

struct Limits {
    std::size_t sets{4'096};
    feasibility::Limits hard{};
};

struct Error {
    enum class Code {
        Input,
        Infeasible,
        Sets,
        Hard,
        Arithmetic,
        Internal,
    };

    Code code;
    std::string message;
    std::vector<Identity> constraints;
    std::optional<feasibility::Error> hard;
};

struct Result {
    Solution solution;
    std::vector<Identity> active;
    std::map<Identity, Expression> predictions;
};

[[nodiscard]] std::expected<Result, Error> solve(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Target> targets,
    Limits limits = {});

} // namespace mq::kernel::pitch::tendency
