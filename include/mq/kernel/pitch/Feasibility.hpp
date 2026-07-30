#pragma once

#include "mq/kernel/pitch/Equation.hpp"
#include "mq/kernel/pitch/Inequality.hpp"
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
};

struct Error {
    enum class Code {
        Input,
        Rows,
        Arithmetic,
        Proof,
    };

    Code code;
    std::string message;
    std::optional<order::Error> proof;
};

struct Report {
    Status status;
    std::vector<std::string> provenance;
    std::optional<order::Proof> proof;
};

[[nodiscard]] std::expected<Report, Error> check(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    Limits limits = {});

} // namespace mq::kernel::pitch::feasibility
