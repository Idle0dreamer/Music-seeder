#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/operation/Operation.hpp"

#include <vector>

namespace mq::kernel::generate {

struct Stage {
    Identity identity;
    std::vector<operation::Any> actions;
};

} // namespace mq::kernel::generate
