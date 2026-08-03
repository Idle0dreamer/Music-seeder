#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>
#include <string>

void test::nahawand_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::Catalog::declared().build_executable("nahawand");
    require(scaffold.has_value(), scaffold.error_or("Nahawand scaffold failed"));
    const eval::Context context{
        .jins = {&scaffold->ajnas},
        .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr},
        .grammar = {},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    mq::kernel::generate::Limits limits;
    limits.timing = test::timing_profile();
    const auto result = engine.run(
        29,
        scaffold->generation.choice,
        scaffold->generation.production,
        scaffold->generation.projection,
        scaffold->generation.schema,
        {},
        limits);
    require(result.has_value(), result ? "" : result.error().message);
    if (result->legal.size() != 3 || !result->rejected.empty()) {
        const auto detail = result->rejected.empty()
            ? (result->derivation.empty()
                   ? std::string("no diagnostic")
                   : result->derivation.front().message)
            : result->rejected.front().message;
        const auto candidate = result->rejected.empty()
            ? std::string{}
            : result->rejected.front().candidate.str();
        require(
            false,
                "Nahawand routes did not all complete: legal=" +
                std::to_string(result->legal.size()) + " " + candidate +
                " " + detail + " diagnostics=" +
                std::to_string(result->derivation.size()));
    }
    require(
        std::ranges::all_of(
            result->legal,
            [](const auto& item) {
                return item.plan.well_formed() &&
                       !item.state.phrase.active &&
                       !item.state.gesture.active &&
                       item.state.cell.owners.size() == item.plan.events.size() &&
                       item.state.phrase.completed.size() >= 1;
            }),
        "Nahawand produced an incomplete plan");
    require(
        std::ranges::all_of(
            result->legal,
            [&](const auto& item) {
                const auto timing = test::timing_profile();
                if (item.plan.events.size() == 1) {
                    return item.plan.events.front().duration ==
                               timing.start.duration &&
                           item.plan.events.front().release &&
                           !item.plan.pauses.empty();
                }
                return item.plan.events.size() == 5 &&
                       item.plan.events[0].onset == Rational(0) &&
                       item.plan.events[0].duration == timing.start.duration &&
                       item.plan.events[1].duration == timing.rise.duration &&
                       std::ranges::all_of(
                           item.plan.events,
                           [](const auto& event) { return event.release.has_value(); }) &&
                       std::ranges::all_of(
                           item.plan.pauses,
                           [](const auto& pause) { return pause.well_formed(); }) &&
                       !item.plan.pauses.empty() &&
                       item.plan.end() >
                           item.plan.events.back().onset +
                               item.plan.events.back().duration;
            }),
        "external timing profile was not applied to Nahawand");
}
