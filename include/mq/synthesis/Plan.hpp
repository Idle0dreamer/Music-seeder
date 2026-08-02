#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/performance/Plan.hpp"
#include "mq/kernel/performance/Timing.hpp"

#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

namespace mq::synthesis {

struct GeneratedPlan {
    ::mq::kernel::Identity candidate;
    ::mq::kernel::performance::Plan plan;
};

[[nodiscard]] std::expected<GeneratedPlan, std::string> make_plan(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing);

} // namespace mq::synthesis
