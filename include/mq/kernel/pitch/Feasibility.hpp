#pragma once

#include "mq/kernel/pitch/Equation.hpp"
#include "mq/kernel/pitch/Inequality.hpp"
#include "mq/kernel/pitch/Solution.hpp"
#include "mq/kernel/pitch/order/Compare.hpp"

#include <expected>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace mq::kernel::pitch::feasibility {

enum class Status {
    Feasible,
    Infeasible,
};

struct Limits {
    std::size_t rows{10'000};
    order::Limits proof{};
    std::size_t retained{100'000};
};

struct Error {
    enum class Code {
        Input,
        Rows,
        Stages,
        Arithmetic,
        Proof,
        Internal,
    };

    Code code;
    std::string message;
    std::optional<order::Error> proof;
};

struct Result {
    Status status;
    std::optional<Solution> solution;
    std::vector<Identity> constraints;
    std::vector<std::string> provenance;
    std::optional<order::Proof> proof;
};

[[nodiscard]] std::expected<Result, Error> analyze(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    Limits limits = {});

} // namespace mq::kernel::pitch::feasibility
