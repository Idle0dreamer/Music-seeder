#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct EventIdTag {};
using EventId = TypedId<EventIdTag>;

} // namespace mq::kernel::sort
