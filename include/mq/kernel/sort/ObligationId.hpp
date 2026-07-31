#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct ObligationIdTag {};
using ObligationId = TypedId<ObligationIdTag>;

} // namespace mq::kernel::sort
