#pragma once

#include "mq/kernel/pitch/tendency/Target.hpp"

namespace mq::kernel::pitch::field {

struct Aim {
    Identity tier;
    tendency::Target target;
};

} // namespace mq::kernel::pitch::field
