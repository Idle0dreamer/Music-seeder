#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Ajam.hpp"

#include <algorithm>

void test::ajam_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_ajam();
    require(scaffold.has_value(), scaffold.error_or("Ajam scaffold failed"));
    const eval::Context context{
        .jins = {&scaffold->ajnas}, .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr}, .grammar = {},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    mq::kernel::generate::Limits limits;
    limits.timing = test::timing_profile();
    const auto result = engine.run(
        41, scaffold->generation.choice, scaffold->generation.production,
        scaffold->generation.projection, scaffold->generation.schema, {}, limits);
    require(result.has_value(), result ? "" : result.error().message);
    require(result->legal.size() == 3 && result->rejected.empty(),
        "Ajam routes did not all complete");
    require(std::ranges::all_of(result->legal, [](const auto& item) {
        return item.plan.well_formed() && !item.state.phrase.active &&
               !item.state.gesture.active;
    }), "Ajam produced an incomplete plan");
}
