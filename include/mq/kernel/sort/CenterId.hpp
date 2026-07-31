#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct CenterIdTag {};
using CenterId = TypedId<CenterIdTag>;

} // namespace mq::kernel::sort
