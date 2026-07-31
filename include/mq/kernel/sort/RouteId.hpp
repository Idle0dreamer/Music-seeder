#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct RouteIdTag {};
using RouteId = TypedId<RouteIdTag>;

} // namespace mq::kernel::sort
