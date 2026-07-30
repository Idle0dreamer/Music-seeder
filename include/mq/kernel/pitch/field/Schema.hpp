#pragma once

#include "mq/kernel/pitch/field/Need.hpp"
#include "mq/kernel/pitch/field/Rule.hpp"

#include <vector>

namespace mq::kernel::pitch::field {

struct Schema {
    std::vector<Identity> variables;
    std::vector<Identity> tiers;
    std::vector<Rule> rules;
    std::vector<Need> needs;
};

} // namespace mq::kernel::pitch::field
