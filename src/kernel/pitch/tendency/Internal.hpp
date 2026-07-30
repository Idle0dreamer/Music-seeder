#pragma once

#include "mq/kernel/pitch/tendency/Solve.hpp"

#include <map>

namespace mq::kernel::pitch::tendency::detail {

using Form = std::map<Identity, Rational>;

struct Model {
    std::vector<Identity> variables;
    std::vector<Equation> equations;
    std::vector<Inequality> inequalities;
    std::vector<Target> targets;
    std::vector<Form> forms;
};

struct Problem {
    std::vector<Identity> variables;
    std::vector<Equation> equations;
    std::vector<Inequality> inequalities;
};

[[nodiscard]] std::expected<Model, Error> model(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Target> targets);

[[nodiscard]] Problem build(
    const Model& model,
    std::span<const std::size_t> active);

[[nodiscard]] std::expected<std::optional<Result>, Error> attempt(
    const Model& model,
    std::span<const std::size_t> active,
    feasibility::Limits limits);

} // namespace mq::kernel::pitch::tendency::detail
