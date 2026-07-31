#include "Internal.hpp"

namespace mq::kernel::pitch::field::assemble::detail {

std::optional<Rational> match(
    const Guard& guard,
    const Facts& context) {
    Rational weight = 1;
    for (const auto& fact : guard.all) {
        const auto found = context.find(fact.key);
        if (found == context.end() || found->second.value != fact.value) {
            return std::nullopt;
        }
        weight *= found->second.weight;
    }
    return weight;
}

} // namespace mq::kernel::pitch::field::assemble::detail
