#include "mq/kernel/motion/Direction.hpp"

namespace mq::kernel::motion {

const char* name(Direction direction) noexcept {
    switch (direction) {
    case Direction::Start:
        return "start";
    case Direction::Same:
        return "same";
    case Direction::Rise:
        return "rise";
    case Direction::Fall:
        return "fall";
    }
    return "unknown";
}

} // namespace mq::kernel::motion
