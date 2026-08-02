#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct StrandIdTag {};
using StrandId = TypedId<StrandIdTag>;

} // namespace mq::kernel::sort
