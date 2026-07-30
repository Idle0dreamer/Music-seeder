#include "Internal.hpp"

#include <stdexcept>

namespace mq::kernel::pitch::feasibility {

std::expected<Report, Error> check(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    Limits limits) {
    try {
        auto rows =
            detail::build(variables, equations, inequalities, limits.rows);
        if (!rows) {
            return std::unexpected(rows.error());
        }
        for (const auto& variable : variables) {
            rows = detail::eliminate(
                std::move(*rows),
                variable,
                limits.rows);
            if (!rows) {
                return std::unexpected(rows.error());
            }
        }

        for (const auto& row : *rows) {
            const auto proof =
                order::compare(Expression{}, row.right, limits.proof);
            if (!proof) {
                return std::unexpected(Error{
                    Error::Code::Proof,
                    proof.error().message,
                    proof.error(),
                });
            }
            if (proof->relation == order::Relation::Greater) {
                return Report{
                    Status::Infeasible,
                    row.provenance,
                    *proof,
                };
            }
        }
        return Report{Status::Feasible, {}, std::nullopt};
    } catch (const std::overflow_error& error) {
        return std::unexpected(Error{
            Error::Code::Arithmetic,
            error.what(),
            std::nullopt,
        });
    }
}

} // namespace mq::kernel::pitch::feasibility
