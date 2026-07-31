#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct CadenceIdTag {};
using CadenceId = TypedId<CadenceIdTag>;

} // namespace mq::kernel::sort
