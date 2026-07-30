#pragma once

#include "mq/kernel/phrase/Cadence.hpp"
#include "mq/kernel/phrase/Function.hpp"

#include <cstddef>
#include <vector>

namespace mq::kernel::phrase {

struct Frame {
    Identity identity;
    Function function;
    std::size_t first{};
    std::vector<Cadence> cadences;

    bool operator==(const Frame&) const = default;
};

} // namespace mq::kernel::phrase
