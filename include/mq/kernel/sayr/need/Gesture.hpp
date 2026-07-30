#pragma once

#include "mq/kernel/Identity.hpp"

#include <cstddef>

namespace mq::kernel::sayr::need {

struct Gesture {
    Identity family;
    std::size_t minimum{1};
};

} // namespace mq::kernel::sayr::need
