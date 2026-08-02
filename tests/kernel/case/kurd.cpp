#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Kurd.hpp"

#include <algorithm>

void test::kurd_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_kurd();
    require(scaffold.has_value(), scaffold.error_or("Kurd scaffold failed"));
    const eval::Context context{
        .jins = {&scaffold->ajnas},
        .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr},
        .grammar = {},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    mq::kernel::generate::Limits limits;
    limits.timing = scaffold->generation.timing;
    const auto result = engine.run(
        37, scaffold->generation.choice, scaffold->generation.production,
        scaffold->generation.projection, scaffold->generation.schema, {}, limits);
    require(result.has_value(), result ? "" : result.error().message);
    require(result->legal.size() == 2 && result->rejected.empty(),
        "Kurd routes did not all complete");
    require(std::ranges::all_of(result->legal, [](const auto& item) {
        return item.plan.well_formed() && !item.state.phrase.active &&
               !item.state.gesture.active &&
               item.state.cell.owners.size() == item.plan.events.size();
    }), "Kurd produced an incomplete plan");
}
