#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

Key keys() {
    return {
        id("key.role"),
        id("key.motion"),
        id("key.region"),
        id("key.baggage"),
        id("key.gesture"),
        id("key.function"),
        id("key.cadence"),
        id("key.boundary"),
        id("motion.start"),
        id("motion.same"),
        id("motion.rise"),
        id("motion.fall"),
        id("boundary.open"),
        id("boundary.closed"),
    };
}

} // namespace mq::kernel::fixture::generation::detail
