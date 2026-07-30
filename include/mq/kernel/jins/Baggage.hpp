#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/motion/Direction.hpp"

#include <set>

namespace mq::kernel::jins {

struct Baggage {
    Identity identity;
    Identity role;
    std::set<Identity> regions;
    std::set<motion::Direction> directions;
    std::set<Identity> gestures;

    bool operator==(const Baggage&) const = default;
};

} // namespace mq::kernel::jins
