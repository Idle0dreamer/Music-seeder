#include "Internal.hpp"

namespace mq::kernel::pitch::tendency::detail {
namespace {

std::expected<Expression, Error> predict(
    const Form& form,
    const Solution& solution) {
    Expression result;
    for (const auto& [variable, coefficient] : form) {
        const auto found = solution.values.find(variable);
        if (found == solution.values.end()) {
            return std::unexpected(Error{
                Error::Code::Internal,
                "KKT result is missing structural variable " +
                    variable.str(),
                {},
                std::nullopt,
            });
        }
        result += found->second * coefficient;
    }
    return result;
}

} // namespace

std::expected<std::optional<Result>, Error> attempt(
    const Model& model,
    std::span<const std::size_t> active,
    feasibility::Limits limits) {
    const auto problem = build(model, active);
    const auto solved = feasibility::analyze(
        problem.variables,
        problem.equations,
        problem.inequalities,
        limits);
    if (!solved) {
        return std::unexpected(Error{
            Error::Code::Hard,
            solved.error().message,
            {},
            solved.error(),
        });
    }
    if (solved->status == feasibility::Status::Infeasible) {
        return std::optional<Result>{};
    }
    if (!solved->solution) {
        return std::unexpected(Error{
            Error::Code::Internal,
            "feasible KKT system did not return a witness",
            {},
            std::nullopt,
        });
    }

    Result result;
    for (const auto& variable : model.variables) {
        const auto found = solved->solution->values.find(variable);
        if (found == solved->solution->values.end()) {
            return std::unexpected(Error{
                Error::Code::Internal,
                "KKT witness is missing structural variable " +
                    variable.str(),
                {},
                std::nullopt,
            });
        }
        result.solution.values.emplace(variable, found->second);
    }
    for (const auto index : active) {
        result.active.push_back(model.inequalities[index].identity);
    }
    for (std::size_t index = 0;
         index < model.targets.size();
         ++index) {
        const auto value = predict(model.forms[index], result.solution);
        if (!value) {
            return std::unexpected(value.error());
        }
        result.predictions.emplace(
            model.targets[index].identity,
            *value);
    }
    return std::optional<Result>{std::move(result)};
}

} // namespace mq::kernel::pitch::tendency::detail
