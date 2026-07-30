#include "mq/kernel/phrase/Boundary.hpp"

namespace mq::kernel::phrase {

const char* name(Boundary boundary) noexcept {
    switch (boundary) {
    case Boundary::Open:
        return "open";
    case Boundary::Closed:
        return "closed";
    }
    return "unknown";
}

} // namespace mq::kernel::phrase
