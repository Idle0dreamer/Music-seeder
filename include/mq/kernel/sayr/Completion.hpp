#pragma once

#include "mq/kernel/sayr/Proof.hpp"
#include "mq/kernel/sort/ObligationId.hpp"

#include <vector>

namespace mq::kernel::sayr {

struct Completion {
    sort::ObligationId obligation;
    std::vector<Proof> proofs;

    bool operator==(const Completion&) const = default;
};

} // namespace mq::kernel::sayr
