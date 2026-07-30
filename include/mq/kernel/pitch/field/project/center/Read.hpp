#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/pitch/field/project/Presence.hpp"

namespace mq::kernel::pitch::field::project::center {

enum class Place {
    Root,
    Local,
};

struct Read {
    Identity key;
    Place place{Place::Local};
    Presence presence{Presence::Required};
};

} // namespace mq::kernel::pitch::field::project::center
