#include "mq/synthesis/Bayati.hpp"
#include "mq/synthesis/Plan.hpp"

namespace mq::synthesis {

std::expected<BayatiPlan, std::string> make_bayati_plan(
    std::uint64_t seed,
    const mq::kernel::performance::Timing& timing) {
    const auto generated = make_plan("bayati", seed, timing);
    if (!generated) {
        return std::unexpected(generated.error());
    }
    return BayatiPlan{generated->candidate, generated->plan};
}

} // namespace mq::synthesis
