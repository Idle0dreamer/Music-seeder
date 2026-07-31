#pragma once

#include "mq/kernel/sort/ObligationId.hpp"

namespace mq::kernel::operation::sayr {

struct Fulfill {
    sort::ObligationId obligation;
};

} // namespace mq::kernel::operation::sayr
