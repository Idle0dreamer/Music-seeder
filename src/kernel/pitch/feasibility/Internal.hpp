#pragma once

#include "mq/kernel/pitch/Feasibility.hpp"

#include <map>

namespace mq::kernel::pitch::feasibility::detail {

struct Row {
    std::map<Identity, Rational> left;
    Expression right;
    std::vector<std::string> provenance;
};

using Rows = std::vector<Row>;

[[nodiscard]] std::expected<Rows, Error> build(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::size_t limit);

[[nodiscard]] std::expected<Rows, Error> eliminate(
    Rows rows,
    const Identity& variable,
    std::size_t limit);

} // namespace mq::kernel::pitch::feasibility::detail
