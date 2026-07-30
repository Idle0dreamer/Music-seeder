#pragma once

#include "mq/kernel/pitch/Expression.hpp"
#include "mq/kernel/pitch/Term.hpp"

#include <string>
#include <vector>

namespace mq::kernel::pitch {

struct Equation {
    Identity identity;
    std::vector<Term> terms;
    Expression right;
    std::string provenance;
};

} // namespace mq::kernel::pitch
