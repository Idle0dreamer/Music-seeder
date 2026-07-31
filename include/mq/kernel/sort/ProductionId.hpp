#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct ProductionIdTag {};
using ProductionId = TypedId<ProductionIdTag>;

} // namespace mq::kernel::sort
