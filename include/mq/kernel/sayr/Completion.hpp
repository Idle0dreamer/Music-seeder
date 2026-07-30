#pragma once

#include "mq/kernel/sayr/Proof.hpp"

#include <vector>

namespace mq::kernel::sayr {

struct Completion {
    Identity obligation;
    std::vector<Proof> proofs;

    bool operator==(const Completion&) const = default;
};

} // namespace mq::kernel::sayr
