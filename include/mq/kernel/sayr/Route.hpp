#pragma once

#include "mq/kernel/Identity.hpp"

#include <set>

namespace mq::kernel::sayr {

struct Route {
    Identity identity;
    std::set<Identity> terminals;
};

} // namespace mq::kernel::sayr
