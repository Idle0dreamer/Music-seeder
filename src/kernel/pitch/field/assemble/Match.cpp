#include "Internal.hpp"

namespace mq::kernel::pitch::field::assemble::detail {

bool match(
    const Guard& guard,
    const Facts& context) {
    for (const auto& fact : guard.all) {
        const auto found = context.find(fact.key);
        if (found == context.end() || found->second != fact.value) {
            return false;
        }
    }
    return true;
}

} // namespace mq::kernel::pitch::field::assemble::detail
