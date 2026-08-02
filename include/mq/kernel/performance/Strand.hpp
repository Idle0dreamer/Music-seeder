#pragma once

#include "mq/kernel/sort/StrandId.hpp"

namespace mq::kernel::performance {

[[nodiscard]] inline sort::StrandId monophonic() {
    return sort::StrandId{"performance.strand", "monophonic", "1"};
}

} // namespace mq::kernel::performance
