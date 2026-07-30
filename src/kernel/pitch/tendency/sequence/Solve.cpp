#include "Internal.hpp"

namespace mq::kernel::pitch::tendency::sequence {
namespace {

Error failure(const tendency::Error& error) {
    return {
        Error::Code::Solve,
        error.message,
        error,
    };
}

} // namespace

std::expected<Result, Error> solve(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Tier> tiers,
    Limits limits) {
    const auto valid = detail::validate(
        variables,
        equations,
        inequalities,
        tiers,
        limits.count);
    if (!valid) {
        return std::unexpected(valid.error());
    }

    std::vector<Equation> hard(equations.begin(), equations.end());
    Result result;
    for (std::size_t index = 0; index < tiers.size(); ++index) {
        const auto& tier = tiers[index];
        auto solved = tendency::solve(
            variables,
            hard,
            inequalities,
            tier.targets,
            limits.tier);
        if (!solved) {
            return std::unexpected(failure(solved.error()));
        }
        result.steps.push_back({
            tier.identity,
            solved->active,
            solved->predictions,
        });
        result.solution = std::move(solved->solution);

        if (index + 1 == tiers.size()) {
            continue;
        }
        for (const auto& target : tier.targets) {
            const auto prediction =
                result.steps.back().predictions.find(target.identity);
            if (prediction == result.steps.back().predictions.end()) {
                return std::unexpected(Error{
                    Error::Code::Internal,
                    "tendency tier result is missing prediction " +
                        target.identity.str(),
                    std::nullopt,
                });
            }
            hard.push_back(detail::freeze(
                tier,
                target,
                prediction->second));
        }
    }
    return result;
}

} // namespace mq::kernel::pitch::tendency::sequence
