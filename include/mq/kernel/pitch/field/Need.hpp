#pragma once

#include "mq/kernel/Identity.hpp"

#include <vector>

namespace mq::kernel::pitch::field {

struct Need {
    Identity identity;
    std::vector<Identity> any;
};

} // namespace mq::kernel::pitch::field
