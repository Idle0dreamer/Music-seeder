#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

namespace mq::kernel::pitch::field {

struct Fact {
    Identity key;
    Identity value;
    Rational weight{1};

    auto operator<=>(const Fact&) const = default;
};

} // namespace mq::kernel::pitch::field
