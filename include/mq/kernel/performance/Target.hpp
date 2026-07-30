#pragma once

#include "mq/kernel/performance/Event.hpp"
#include "mq/kernel/pitch/Expression.hpp"

namespace mq::kernel::performance {

struct Target {
    Event event;
    pitch::Expression center;

    bool operator==(const Target&) const = default;
};

} // namespace mq::kernel::performance
