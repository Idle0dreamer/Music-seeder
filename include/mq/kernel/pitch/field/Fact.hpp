#pragma once

#include "mq/kernel/Identity.hpp"

namespace mq::kernel::pitch::field {

struct Fact {
    Identity key;
    Identity value;

    auto operator<=>(const Fact&) const = default;
};

} // namespace mq::kernel::pitch::field
