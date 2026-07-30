#include "Internal.hpp"

namespace mq::kernel::pitch::feasibility::detail {
namespace {

std::expected<Expression, Error> evaluate(
    std::span<const Term> terms,
    const Solution& solution) {
    Expression result;
    for (const auto& term : terms) {
        const auto found = solution.values.find(term.variable);
        if (found == solution.values.end()) {
            return std::unexpected(Error{
                Error::Code::Internal,
                "pitch witness validation is missing a declared variable",
                std::nullopt,
            });
        }
        result += found->second * term.coefficient;
    }
    return result;
}

Error invalid(const std::string& provenance) {
    return Error{
        Error::Code::Internal,
        "constructed pitch witness violates " + provenance,
        std::nullopt,
    };
}

} // namespace

std::expected<void, Error> validate(
    const Solution& solution,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    order::Limits limits) {
    for (const auto& equation : equations) {
        const auto left = evaluate(equation.terms, solution);
        if (!left) {
            return std::unexpected(left.error());
        }
        if (*left != equation.right) {
            return std::unexpected(invalid(equation.provenance));
        }
    }
    for (const auto& inequality : inequalities) {
        const auto left = evaluate(inequality.terms, solution);
        if (!left) {
            return std::unexpected(left.error());
        }
        const auto proof = order::compare(*left, inequality.right, limits);
        if (!proof) {
            return std::unexpected(Error{
                Error::Code::Proof,
                proof.error().message,
                proof.error(),
            });
        }
        if (proof->relation == order::Relation::Greater) {
            return std::unexpected(invalid(inequality.provenance));
        }
    }
    return {};
}

} // namespace mq::kernel::pitch::feasibility::detail
