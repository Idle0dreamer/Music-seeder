#include "mq/kernel/pitch/field/solve/Run.hpp"

namespace mq::kernel::pitch::field::solve {

std::expected<Result, Error> run(
    const Schema& schema,
    const Context& context,
    Limits limits) {
    auto model = assemble::run(schema, context, limits.assembly);
    if (!model) {
        return std::unexpected(Error{
            Error::Code::Assembly,
            model.error().message,
            model.error(),
            std::nullopt,
            std::nullopt,
        });
    }

    if (!model->tiers.empty()) {
        auto solved = tendency::sequence::solve(
            model->variables,
            model->equations,
            model->inequalities,
            model->tiers,
            limits.tendency);
        if (!solved) {
            const bool infeasible =
                solved.error().cause &&
                solved.error().cause->code ==
                    tendency::Error::Code::Infeasible;
            return std::unexpected(Error{
                infeasible
                    ? Error::Code::Hard
                    : Error::Code::Tendency,
                solved.error().message,
                std::nullopt,
                std::nullopt,
                solved.error(),
            });
        }
        return Result{
            std::move(solved->solution),
            std::move(solved->steps),
            std::move(model->rules),
        };
    }

    auto solved = feasibility::analyze(
        model->variables,
        model->equations,
        model->inequalities,
        limits.hard);
    if (!solved) {
        return std::unexpected(Error{
            Error::Code::Hard,
            solved.error().message,
            std::nullopt,
            solved.error(),
            std::nullopt,
        });
    }
    if (solved->status == feasibility::Status::Infeasible) {
        return std::unexpected(Error{
            Error::Code::Hard,
            "assembled hard pitch field is infeasible",
            std::nullopt,
            std::nullopt,
            std::nullopt,
        });
    }
    if (!solved->solution) {
        return std::unexpected(Error{
            Error::Code::Internal,
            "feasible field result has no exact witness",
            std::nullopt,
            std::nullopt,
            std::nullopt,
        });
    }
    return Result{
        std::move(*solved->solution),
        {},
        std::move(model->rules),
    };
}

} // namespace mq::kernel::pitch::field::solve
