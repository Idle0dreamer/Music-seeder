#pragma once

#include "mq/kernel/choice/Select.hpp"
#include "mq/kernel/generate/Stage.hpp"

namespace mq::kernel::generate {

struct Candidate {
    Identity identity;
    choice::Cost cost;
    std::vector<Stage> stages;
};

} // namespace mq::kernel::generate
