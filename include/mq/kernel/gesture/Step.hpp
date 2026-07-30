#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/motion/Direction.hpp"

#include <optional>
#include <set>

namespace mq::kernel::gesture {

struct Step {
    std::set<Identity> roles;
    std::set<Identity> regions;
    std::set<motion::Direction> directions;
    std::optional<Identity> baggage;

    bool operator==(const Step&) const = default;
};

} // namespace mq::kernel::gesture
