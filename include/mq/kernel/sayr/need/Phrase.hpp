#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/phrase/Boundary.hpp"

#include <cstddef>
#include <optional>

namespace mq::kernel::sayr::need {

struct Phrase {
    Identity function;
    phrase::Boundary boundary{phrase::Boundary::Open};
    std::optional<Identity> cadence;
    std::size_t minimum{1};
};

} // namespace mq::kernel::sayr::need
