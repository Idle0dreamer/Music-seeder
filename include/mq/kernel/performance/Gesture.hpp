#pragma once

#include "mq/kernel/Identity.hpp"

namespace mq::kernel::performance {

struct Gesture {
    Identity occurrence;
    Identity family;

    bool operator==(const Gesture&) const = default;
};

} // namespace mq::kernel::performance
