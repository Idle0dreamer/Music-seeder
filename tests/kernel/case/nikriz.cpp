#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Nikriz.hpp"

#include <algorithm>

void test::nikriz_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_nikriz();
    require(scaffold.has_value(), scaffold.error_or("Nikriz scaffold failed"));
    const eval::Context context{
        .jins = {&scaffold->ajnas}, .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr}, .grammar = {},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    mq::kernel::generate::Limits limits;
    limits.timing = scaffold->generation.timing;
    const auto result = engine.run(
        43, scaffold->generation.choice, scaffold->generation.production,
        scaffold->generation.projection, scaffold->generation.schema, {}, limits);
    require(result.has_value(), result ? "" : result.error().message);
    require(result->legal.size() == 2 && result->rejected.empty(),
        "Nikriz routes did not all complete");
    require(std::ranges::all_of(result->legal, [](const auto& item) {
        return item.plan.well_formed() && !item.state.phrase.active &&
               !item.state.gesture.active;
    }), "Nikriz produced an incomplete plan");
}
