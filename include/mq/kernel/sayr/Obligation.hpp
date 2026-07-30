#pragma once

#include "mq/kernel/sayr/Need.hpp"

#include <set>
#include <vector>

namespace mq::kernel::sayr {

struct Obligation {
    Identity identity;
    std::vector<Need> needs;
    std::set<Identity> after;
};

} // namespace mq::kernel::sayr
