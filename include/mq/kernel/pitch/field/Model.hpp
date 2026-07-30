#pragma once

#include "mq/kernel/pitch/Equation.hpp"
#include "mq/kernel/pitch/Inequality.hpp"
#include "mq/kernel/pitch/tendency/Tier.hpp"

#include <vector>

namespace mq::kernel::pitch::field {

struct Model {
    std::vector<Identity> variables;
    std::vector<Equation> equations;
    std::vector<Inequality> inequalities;
    std::vector<tendency::Tier> tiers;
    std::vector<Identity> rules;
};

} // namespace mq::kernel::pitch::field
