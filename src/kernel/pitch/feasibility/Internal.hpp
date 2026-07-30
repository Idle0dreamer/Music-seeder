#pragma once

#include "mq/kernel/pitch/Feasibility.hpp"

#include <map>

namespace mq::kernel::pitch::feasibility::detail {

struct Row {
    std::map<Identity, Rational> left;
    Expression right;
    std::vector<Identity> constraints;
    std::vector<std::string> provenance;
};

using Rows = std::vector<Row>;

struct Projection {
    std::vector<Rows> stages;
    Rows final;
};

[[nodiscard]] std::expected<void, Error> identities(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities);

[[nodiscard]] std::expected<Rows, Error> build(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::size_t limit);

[[nodiscard]] std::expected<Rows, Error> eliminate(
    Rows rows,
    const Identity& variable,
    std::size_t limit);

[[nodiscard]] std::expected<Projection, Error> project(
    Rows rows,
    std::span<const Identity> variables,
    Limits limits);

[[nodiscard]] std::expected<Solution, Error> restore(
    const Projection& projection,
    std::span<const Identity> variables,
    order::Limits limits);

[[nodiscard]] std::expected<void, Error> validate(
    const Solution& solution,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    order::Limits limits);

} // namespace mq::kernel::pitch::feasibility::detail
