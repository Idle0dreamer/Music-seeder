#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct RegionIdTag {};
using RegionId = TypedId<RegionIdTag>;

} // namespace mq::kernel::sort
