#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct MaqamIdTag {};
using MaqamId = TypedId<MaqamIdTag>;

} // namespace mq::kernel::sort
