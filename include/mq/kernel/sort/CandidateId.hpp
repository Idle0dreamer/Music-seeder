#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct CandidateIdTag {};
using CandidateId = TypedId<CandidateIdTag>;

} // namespace mq::kernel::sort
