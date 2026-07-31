#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct BaggageIdTag {};
using BaggageId = TypedId<BaggageIdTag>;

} // namespace mq::kernel::sort
