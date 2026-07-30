#pragma once

#include "mq/kernel/Identity.hpp"

namespace mq::kernel::operation::gesture {

struct Begin {
    Identity occurrence;
    Identity family;
};

} // namespace mq::kernel::operation::gesture
