#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

namespace mq::kernel::pitch {

struct Term {
    Identity variable;
    Rational coefficient;
};

} // namespace mq::kernel::pitch
