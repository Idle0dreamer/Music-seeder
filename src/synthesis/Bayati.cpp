#include "mq/synthesis/Bayati.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Bayati.hpp"

#include <algorithm>

namespace mq::synthesis {

std::expected<BayatiPlan, std::string> make_bayati_plan(
    std::uint64_t seed,
    const mq::kernel::performance::Timing& timing) {
    const auto scaffold = mq::kernel::maqam::make_bayati();
    if (!scaffold) {
        return std::unexpected(scaffold.error());
    }
    const mq::kernel::eval::Context context{
        .jins = {&scaffold->ajnas},
        .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr},
        .grammar = {},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    const auto generated = engine.run(
        seed,
        scaffold->generation.choice,
        scaffold->generation.production,
        scaffold->generation.projection,
        scaffold->generation.schema,
        {},
        mq::kernel::generate::Limits{
            .timing = timing,
        });
    if (!generated) {
        return std::unexpected(generated.error().message);
    }
    const auto selected = std::ranges::find(
        generated->legal,
        generated->selected,
        &mq::kernel::generate::Outcome::candidate);
    if (selected == generated->legal.end()) {
        return std::unexpected("selected Bayati outcome is missing");
    }
    return BayatiPlan{generated->selected, selected->plan};
}

} // namespace mq::synthesis
