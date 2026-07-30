#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/pitch/Expression.hpp"

#include <map>

namespace mq::kernel::pitch {

struct Solution {
    std::map<Identity, Expression> values;
};

} // namespace mq::kernel::pitch
