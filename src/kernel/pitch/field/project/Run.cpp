#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::pitch::field::project {

std::expected<Context, Error> run(
    const Plan& plan,
    const state::Snapshot& state,
    Limits limits) {
    const auto valid = detail::check(plan, limits);
    if (!valid) {
        return std::unexpected(valid.error());
    }

    Context result;
    result.facts.reserve(plan.sources.size());
    for (const auto& source : plan.sources) {
        const auto fact = detail::read(source, state);
        if (!fact) {
            return std::unexpected(fact.error());
        }
        if (*fact) {
            result.facts.push_back(std::move(**fact));
        }
    }
    std::ranges::sort(result.facts);
    return result;
}

} // namespace mq::kernel::pitch::field::project
