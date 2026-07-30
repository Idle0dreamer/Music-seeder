#pragma once

#include "mq/kernel/pitch/field/project/Source.hpp"

#include <vector>

namespace mq::kernel::pitch::field::project {

struct Plan {
    std::vector<Source> sources;
};

} // namespace mq::kernel::pitch::field::project
