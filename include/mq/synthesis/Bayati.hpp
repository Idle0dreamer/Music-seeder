#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/performance/Plan.hpp"

#include <cstdint>
#include <expected>
#include <string>

namespace mq::synthesis {

struct BayatiPlan {
    ::mq::kernel::Identity candidate;
    ::mq::kernel::performance::Plan plan;
};

[[nodiscard]] std::expected<BayatiPlan, std::string> make_bayati_plan(
    std::uint64_t seed);

} // namespace mq::synthesis
