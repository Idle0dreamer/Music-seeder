#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/pitch/field/project/Presence.hpp"

namespace mq::kernel::pitch::field::project::motion {

struct Read {
    Identity key;
    Identity start;
    Identity same;
    Identity rise;
    Identity fall;
    Presence presence{Presence::Required};
};

} // namespace mq::kernel::pitch::field::project::motion
