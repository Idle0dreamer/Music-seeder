#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/motion/Direction.hpp"

namespace mq::kernel::performance {

struct Event {
    Identity identity;
    Identity role;
    motion::Direction direction{motion::Direction::Start};

    bool operator==(const Event&) const = default;
};

} // namespace mq::kernel::performance
