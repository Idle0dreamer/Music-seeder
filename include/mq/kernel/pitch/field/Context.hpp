#pragma once

#include "mq/kernel/pitch/field/Fact.hpp"

#include <vector>

namespace mq::kernel::pitch::field {

struct Context {
    std::vector<Fact> facts;
};

} // namespace mq::kernel::pitch::field
