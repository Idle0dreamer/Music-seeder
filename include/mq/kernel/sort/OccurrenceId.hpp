#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct OccurrenceIdTag {};
using OccurrenceId = TypedId<OccurrenceIdTag>;

} // namespace mq::kernel::sort
