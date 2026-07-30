#pragma once

#include "mq/kernel/Identity.hpp"

#include <cstddef>

namespace mq::kernel::gesture {

struct Frame {
    Identity occurrence;
    Identity family;
    std::size_t first{};
    std::size_t next{};

    bool operator==(const Frame&) const = default;
};

} // namespace mq::kernel::gesture
