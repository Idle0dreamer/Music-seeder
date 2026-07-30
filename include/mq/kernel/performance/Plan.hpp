#pragma once

#include "mq/kernel/performance/Target.hpp"

#include <vector>

namespace mq::kernel::performance {

struct Plan {
    std::vector<Target> targets;
};

} // namespace mq::kernel::performance
