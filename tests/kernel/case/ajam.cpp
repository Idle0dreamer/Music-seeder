#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Catalog.hpp"

#include <string>

#include <algorithm>

void test::ajam_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::Catalog::declared().build_executable("ajam");
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
    if (result->legal.size() != 3 || !result->rejected.empty()) {
        std::string detail =
            "Ajam routes did not all complete: legal=" +
            std::to_string(result->legal.size());
        for (const auto& rejection : result->rejected) {
            detail += "\n" + rejection.candidate.str() + ": " +
                      rejection.message;
        }
        require(false, detail);
    }
    require(std::ranges::all_of(result->legal, [](const auto& item) {
        return item.plan.well_formed() && !item.state.phrase.active &&
               !item.state.gesture.active;
    }), "Ajam produced an incomplete plan");
}
