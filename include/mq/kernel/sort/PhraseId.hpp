#pragma once

#include "mq/kernel/sort/TypedId.hpp"

namespace mq::kernel::sort {

struct PhraseIdTag {};
using PhraseId = TypedId<PhraseIdTag>;

} // namespace mq::kernel::sort
