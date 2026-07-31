#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct GestureIdTag {};
using GestureId = TypedId<GestureIdTag>;

} // namespace mq::kernel::sort
