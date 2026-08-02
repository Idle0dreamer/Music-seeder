#pragma once

#include "mq/kernel/performance/Plan.hpp"
#include "mq/kernel/pitch/field/project/Run.hpp"
#include "mq/kernel/pitch/field/solve/Run.hpp"
#include "mq/kernel/pitch/order/Compare.hpp"

namespace mq::kernel::pitch::request {

struct Limits {
    std::size_t history{4'096};
    field::project::Limits projection{};
    field::solve::Limits field{};
    order::Limits order{};
};

struct Error {
    enum class Code {
        State,
        Count,
        Project,
        Field,
        History,
        Plan,
        Role,
        Direction,
        Order,
    };

    Code code;
    std::string message;
    std::optional<field::project::Error> projection;
    std::optional<field::solve::Error> field;
    std::optional<order::Error> order;
};

struct Result {
    performance::Plan plan;
    field::Context context;
    field::solve::Result field;
    std::optional<order::Proof> direction;
};

[[nodiscard]] std::expected<Result, Error> run(
    const state::Snapshot& state,
    const field::project::Plan& projection,
    const field::Schema& schema,
    performance::Plan prefix = {},
    Limits limits = {});

} // namespace mq::kernel::pitch::request
