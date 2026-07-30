#pragma once

#include "mq/kernel/pitch/Term.hpp"
#include "mq/kernel/pitch/Expression.hpp"

#include <string>
#include <vector>

namespace mq::kernel::pitch::tendency {

struct Target {
    Identity identity;
    std::vector<Term> terms;
    Expression target;
    Rational weight{1};
    std::string provenance;
};

} // namespace mq::kernel::pitch::tendency
