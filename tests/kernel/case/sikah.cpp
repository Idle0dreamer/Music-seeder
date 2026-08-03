#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Sikah.hpp"

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
        "Sikah ordered formula alternatives were not both legal candidates");
    require(
        result->legal.front().plan.events.size() == 5 &&
            result->legal.front().plan.well_formed() &&
            result->legal.back().plan.events.size() == 5 &&
            result->legal.back().plan.well_formed(),
        "Sikah ordered formula alternatives did not produce complete plans");
    require(
        [&] {
            const auto& left = result->legal.front();
            const auto& right = result->legal.back();
            const auto left_owner = left.state.cell.owners.at(
                sort::EventId{left.plan.events[3].target.event.identity});
            const auto right_owner = right.state.cell.owners.at(
                sort::EventId{right.plan.events[3].target.event.identity});
            return left_owner.variation != right_owner.variation &&
                   left.plan.events[3].ornament &&
                   right.plan.events[3].ornament &&
                   left.plan.events[3].ornament->kind !=
                       right.plan.events[3].ornament->kind;
        }(),
        "Sikah formula alternatives did not change stored formula and performance realization");
}
