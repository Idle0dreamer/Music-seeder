#include "Internal.hpp"

#include <stdexcept>

namespace mq::kernel::pitch::feasibility {

std::expected<Result, Error> analyze(
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
        const auto projection =
            detail::project(std::move(*rows), variables, limits);
        if (!projection) {
            return std::unexpected(projection.error());
        }
        for (const auto& row : projection->final) {
            if (!row.left.empty()) {
                return std::unexpected(Error{
                    Error::Code::Internal,
                    "pitch feasibility projection retained a variable",
                    std::nullopt,
                });
            }
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
                return Result{
                    Status::Infeasible,
                    std::nullopt,
                    row.constraints,
                    row.provenance,
                    *proof,
                };
            }
        }

        auto solution =
            detail::restore(*projection, variables, limits.proof);
        if (!solution) {
            return std::unexpected(solution.error());
        }
        const auto valid =
            detail::validate(*solution, equations, inequalities, limits.proof);
        if (!valid) {
            return std::unexpected(valid.error());
        }
        return Result{
            Status::Feasible,
            std::move(*solution),
            {},
            {},
            std::nullopt,
        };
    } catch (const std::overflow_error& error) {
        return std::unexpected(Error{
            Error::Code::Arithmetic,
            error.what(),
            std::nullopt,
        });
    }
}

} // namespace mq::kernel::pitch::feasibility
