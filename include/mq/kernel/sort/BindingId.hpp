#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct BindingIdTag {};
using BindingId = TypedId<BindingIdTag>;

} // namespace mq::kernel::sort
