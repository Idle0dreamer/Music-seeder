#include "Internal.hpp"

#include <stdexcept>

namespace mq::kernel::pitch::tendency {
namespace {

class Search {
public:
    Search(
        const detail::Model& model,
        Limits limits)
        : model_(model),
          limits_(limits) {}

    std::expected<std::optional<Result>, Error> run(
        std::size_t start,
        std::size_t remaining) {
        if (remaining == 0) {
            if (attempts_ >= limits_.sets) {
                return std::unexpected(Error{
                    Error::Code::Sets,
                    "tendency active-set budget exceeded",
                    {},
                    std::nullopt,
                });
            }
            ++attempts_;
            return detail::attempt(model_, active_, limits_.hard);
        }
        const auto count = model_.inequalities.size();
        for (std::size_t index = start;
             index + remaining <= count;
             ++index) {
            active_.push_back(index);
            auto result = run(index + 1, remaining - 1);
            active_.pop_back();
            if (!result) {
                return std::unexpected(result.error());
            }
            if (*result) {
                return result;
            }
        }
        return std::optional<Result>{};
    }

private:
    const detail::Model& model_;
    Limits limits_;
    std::size_t attempts_{};
    std::vector<std::size_t> active_;
};

Error hard(const feasibility::Error& error) {
    return {
        Error::Code::Hard,
        error.message,
        {},
        error,
    };
}

} // namespace

std::expected<Result, Error> solve(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Target> targets,
    Limits limits) {
    try {
        const auto normalized =
            detail::model(variables, equations, inequalities, targets);
        if (!normalized) {
            return std::unexpected(normalized.error());
        }
        const auto feasible = feasibility::analyze(
            normalized->variables,
            normalized->equations,
            normalized->inequalities,
            limits.hard);
        if (!feasible) {
            return std::unexpected(hard(feasible.error()));
        }
        if (feasible->status == feasibility::Status::Infeasible) {
            return std::unexpected(Error{
                Error::Code::Infeasible,
                "hard pitch region is infeasible",
                feasible->constraints,
                std::nullopt,
            });
        }

        Search search(*normalized, limits);
        for (std::size_t size = 0;
             size <= normalized->inequalities.size();
             ++size) {
            auto result = search.run(0, size);
            if (!result) {
                return std::unexpected(result.error());
            }
            if (*result) {
                return std::move(**result);
            }
        }
        return std::unexpected(Error{
            Error::Code::Internal,
            "feasible convex tendency tier has no KKT witness",
            {},
            std::nullopt,
        });
    } catch (const std::overflow_error& error) {
        return std::unexpected(Error{
            Error::Code::Arithmetic,
            error.what(),
            {},
            std::nullopt,
        });
    }
}

} // namespace mq::kernel::pitch::tendency
