#pragma once

#include "mq/kernel/performance/Timing.hpp"
#include "mq/kernel/pitch/request/Run.hpp"

#include <cstddef>
#include <optional>

namespace mq::kernel::generate {

struct Limits {
    std::size_t candidates{256};
    std::size_t stages{4'096};
    std::size_t actions{64};
    std::optional<performance::Timing> timing;
    pitch::request::Limits pitch{};
};

} // namespace mq::kernel::generate
