#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/performance/Plan.hpp"
#include "mq/kernel/performance/Timing.hpp"

#include <cstdint>
#include <expected>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace mq::synthesis {

struct GeneratedPlan {
    ::mq::kernel::Identity candidate;
    ::mq::kernel::performance::Plan plan;
    std::vector<::mq::kernel::Identity> phrase_candidates;
};

[[nodiscard]] std::expected<GeneratedPlan, std::string> make_plan(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    std::size_t repetitions = 1);

void append_plan(
    ::mq::kernel::performance::Plan& destination,
    const ::mq::kernel::performance::Plan& continuation);

} // namespace mq::synthesis
