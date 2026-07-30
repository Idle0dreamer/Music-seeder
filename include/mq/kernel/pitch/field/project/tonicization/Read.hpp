#pragma once

#include "mq/kernel/Identity.hpp"

namespace mq::kernel::pitch::field::project::tonicization {

struct Read {
    Identity key;
    Identity color;
    Identity internal;
    Identity maqam;
};

} // namespace mq::kernel::pitch::field::project::tonicization
