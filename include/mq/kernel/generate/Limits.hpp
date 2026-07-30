#pragma once

#include "mq/kernel/pitch/request/Run.hpp"

#include <cstddef>

namespace mq::kernel::generate {

struct Limits {
    std::size_t candidates{256};
    std::size_t stages{4'096};
    std::size_t actions{64};
    pitch::request::Limits pitch{};
};

} // namespace mq::kernel::generate
