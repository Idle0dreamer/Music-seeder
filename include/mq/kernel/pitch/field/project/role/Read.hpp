#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/pitch/field/project/Presence.hpp"

namespace mq::kernel::pitch::field::project::role {

struct Read {
    Identity key;
    Presence presence{Presence::Required};
};

} // namespace mq::kernel::pitch::field::project::role
