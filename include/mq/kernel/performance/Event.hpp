#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/motion/Direction.hpp"
#include "mq/kernel/performance/Gesture.hpp"

#include <optional>

namespace mq::kernel::performance {

struct Event {
    Identity identity;
    Identity role;
    motion::Direction direction{motion::Direction::Start};
    Identity region;
    std::optional<Identity> baggage;
    std::optional<Gesture> gesture;

    bool operator==(const Event&) const = default;
};

} // namespace mq::kernel::performance
