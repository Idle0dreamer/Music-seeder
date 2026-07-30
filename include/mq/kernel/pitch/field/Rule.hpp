#pragma once

#include "mq/kernel/pitch/field/Effect.hpp"
#include "mq/kernel/pitch/field/Guard.hpp"

namespace mq::kernel::pitch::field {

struct Rule {
    Guard when;
    Effect effect;
};

} // namespace mq::kernel::pitch::field
