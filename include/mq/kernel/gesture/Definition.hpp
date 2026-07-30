#pragma once

#include "mq/kernel/gesture/Step.hpp"

#include <vector>

namespace mq::kernel::gesture {

struct Definition {
    Identity identity;
    std::vector<Step> steps;

    bool operator==(const Definition&) const = default;
};

} // namespace mq::kernel::gesture
