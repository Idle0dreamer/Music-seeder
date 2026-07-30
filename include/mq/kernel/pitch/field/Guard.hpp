#pragma once

#include "mq/kernel/pitch/field/Fact.hpp"

#include <vector>

namespace mq::kernel::pitch::field {

struct Guard {
    std::vector<Fact> all;
};

} // namespace mq::kernel::pitch::field
