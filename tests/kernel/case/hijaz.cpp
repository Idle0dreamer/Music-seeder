#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Hijaz.hpp"

#include <algorithm>
#include <string>

void test::hijaz_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_hijaz();
    require(scaffold.has_value(), scaffold.error_or("Hijaz scaffold failed"));
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
        31, scaffold->generation.choice, scaffold->generation.production,
        scaffold->generation.projection, scaffold->generation.schema, {}, limits);
    require(result.has_value(), result ? "" : result.error().message);
    require(result->legal.size() == 3 && result->rejected.empty(),
        "Hijaz routes did not all complete");
    require(std::ranges::all_of(result->legal, [](const auto& item) {
        return item.plan.well_formed() && !item.state.phrase.active &&
               !item.state.gesture.active &&
               item.state.cell.owners.size() == item.plan.events.size();
    }), "Hijaz produced an incomplete plan");
}
