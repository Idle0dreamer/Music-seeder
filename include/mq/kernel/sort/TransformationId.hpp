#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct TransformationIdTag {};
using TransformationId = TypedId<TransformationIdTag>;

} // namespace mq::kernel::sort
