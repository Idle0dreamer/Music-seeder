#pragma once

#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"

namespace mq::kernel::sayr::need {

struct Evidence {
    evidence::Kind kind;
    Rational minimum{1};
};

} // namespace mq::kernel::sayr::need
