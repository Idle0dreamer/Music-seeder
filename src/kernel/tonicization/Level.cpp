#include "mq/kernel/tonicization/Level.hpp"

namespace mq::kernel::tonicization {

const char* name(Level value) noexcept {
    switch (value) {
    case Level::Color:
        return "color";
    case Level::Internal:
        return "internal";
    case Level::Maqam:
        return "maqam";
    }
    return "unknown";
}

} // namespace mq::kernel::tonicization
