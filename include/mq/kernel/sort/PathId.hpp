#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct PathIdTag {};
using PathId = TypedId<PathIdTag>;

} // namespace mq::kernel::sort
