#pragma once

#include "mq/kernel/sayr/proof/Item.hpp"

namespace mq::kernel::sayr {

struct Proof {
    Identity need;
    proof::Item value;

    bool operator==(const Proof&) const = default;
};

} // namespace mq::kernel::sayr
