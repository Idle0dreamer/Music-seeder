#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Sikah.hpp"

#include <algorithm>
#include <string>

void test::sikah_case() {
    using namespace mq::kernel;
    const auto scaffold = maqam::make_sikah();
    require(scaffold.has_value(), scaffold.error_or("Sikah scaffold failed"));
    const eval::Context context{
        .jins = {&scaffold->ajnas}, .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr}, .grammar = {},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    mq::kernel::generate::Limits limits;
    limits.timing = test::timing_profile();
    const auto result = engine.run(
        53,
        scaffold->generation.choice,
        scaffold->generation.production,
        scaffold->generation.projection,
        scaffold->generation.schema,
        {},
        limits);
    if (!result) {
        std::string detail = result.error().message;
        for (const auto& rejection : result.error().rejected) {
            detail += "\n" + rejection.candidate.str() + ": " +
                      rejection.message;
        }
        for (const auto& diagnostic : result.error().derivation) {
            detail += "\n" + diagnostic.term.str() + ": " +
                      diagnostic.message;
        }
        require(false, detail);
    }
    require(
        result->legal.size() == 2 && result->rejected.empty(),
        "Sikah ordered station routes did not expose both legal station orders");
    require(
        std::ranges::all_of(
            result->legal,
            [](const auto& item) {
                return item.plan.events.size() == 5 &&
                       item.plan.well_formed();
            }),
        "Sikah ordered routes did not produce complete timed plans");
    require(
        result->legal[0].candidate != result->legal[1].candidate,
        "Sikah ordered routes collapsed to one candidate identity");
}
