#pragma once

#include "mq/kernel/Identity.hpp"

namespace mq::kernel::gesture {

struct Span {
    Identity occurrence;
    Identity family;
    Identity first;
    Identity last;

    bool operator==(const Span&) const = default;
};

} // namespace mq::kernel::gesture
