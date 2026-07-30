#include "Internal.hpp"

namespace mq::kernel::pitch::feasibility::detail {
namespace {

std::expected<order::Relation, Error> relation(
    const Expression& left,
    const Expression& right,
    order::Limits limits) {
    const auto proof = order::compare(left, right, limits);
    if (!proof) {
        return std::unexpected(Error{
            Error::Code::Proof,
            proof.error().message,
            proof.error(),
        });
    }
    return proof->relation;
}

std::expected<Expression, Error> bound(
    const Row& row,
    const Identity& variable,
    const Solution& solution) {
    Expression remainder;
    for (const auto& [identity, coefficient] : row.left) {
        if (identity == variable) {
            continue;
        }
        const auto found = solution.values.find(identity);
        if (found == solution.values.end()) {
            return std::unexpected(Error{
                Error::Code::Internal,
                "pitch witness restoration encountered an unresolved variable",
                std::nullopt,
            });
        }
        remainder += found->second * coefficient;
    }
    const auto coefficient = row.left.at(variable);
    return (row.right - remainder) *
           (Rational(1) / coefficient);
}

} // namespace

std::expected<Solution, Error> restore(
    const Projection& projection,
    std::span<const Identity> variables,
    order::Limits limits) {
    if (projection.stages.size() != variables.size()) {
        return std::unexpected(Error{
            Error::Code::Internal,
            "pitch witness projection does not match variable count",
            std::nullopt,
        });
    }

    Solution solution;
    for (auto index = variables.size(); index-- > 0;) {
        const auto& variable = variables[index];
        std::optional<Expression> lower;
        std::optional<Expression> upper;
        for (const auto& row : projection.stages[index]) {
            const auto found = row.left.find(variable);
            if (found == row.left.end()) {
                continue;
            }
            const auto candidate = bound(row, variable, solution);
            if (!candidate) {
                return std::unexpected(candidate.error());
            }
            if (found->second < Rational(0)) {
                if (!lower) {
                    lower = *candidate;
                } else {
                    const auto ordering =
                        relation(*candidate, *lower, limits);
                    if (!ordering) {
                        return std::unexpected(ordering.error());
                    }
                    if (*ordering == order::Relation::Greater) {
                        lower = *candidate;
                    }
                }
            } else if (!upper) {
                upper = *candidate;
            } else {
                const auto ordering =
                    relation(*candidate, *upper, limits);
                if (!ordering) {
                    return std::unexpected(ordering.error());
                }
                if (*ordering == order::Relation::Less) {
                    upper = *candidate;
                }
            }
        }
        if (lower && upper) {
            const auto ordering = relation(*lower, *upper, limits);
            if (!ordering) {
                return std::unexpected(ordering.error());
            }
            if (*ordering == order::Relation::Greater) {
                return std::unexpected(Error{
                    Error::Code::Internal,
                    "pitch witness bounds contradict a feasible projection",
                    std::nullopt,
                });
            }
        }
        solution.values.emplace(
            variable,
            lower ? *lower : upper ? *upper : Expression{});
    }
    return solution;
}

} // namespace mq::kernel::pitch::feasibility::detail
