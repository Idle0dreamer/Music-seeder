#pragma once

#include "mq/kernel/pitch/tendency/Target.hpp"

namespace mq::kernel::pitch::tendency {

struct Tier {
    Identity identity;
    std::vector<Target> targets;
};

} // namespace mq::kernel::pitch::tendency
