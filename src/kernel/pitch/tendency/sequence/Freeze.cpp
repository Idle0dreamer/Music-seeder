#include "Internal.hpp"

namespace mq::kernel::pitch::tendency::sequence::detail {

Equation freeze(
    const Tier& tier,
    const Target& target,
    const Expression& prediction) {
    return {
        {
            "mq.kernel.sequence.freeze",
            target.identity.str(),
            "1",
        },
        target.terms,
        prediction,
        "preserve tendency tier " + tier.identity.str(),
    };
}

} // namespace mq::kernel::pitch::tendency::sequence::detail
