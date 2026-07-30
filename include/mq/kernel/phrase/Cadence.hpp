#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

namespace mq::kernel::phrase {

struct Cadence {
    Identity family;
    Identity event;
    Rational strength{1};

    bool operator==(const Cadence&) const = default;
};

} // namespace mq::kernel::phrase
