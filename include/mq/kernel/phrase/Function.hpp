#pragma once

#include "mq/kernel/Identity.hpp"

namespace mq::kernel::phrase {

struct Function {
    Identity identity;

    bool operator==(const Function&) const = default;
};

} // namespace mq::kernel::phrase
