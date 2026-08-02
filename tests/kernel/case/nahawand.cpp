#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Nahawand.hpp"

#include <algorithm>
#include <string>

void test::nahawand_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_nahawand();
    require(scaffold.has_value(), scaffold.error_or("Nahawand scaffold failed"));
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
                const auto& timing = scaffold->generation.timing;
                if (item.plan.events.size() == 1) {
                    return item.plan.events.front().duration ==
                        timing.start.duration;
                }
                return item.plan.events.size() == 5 &&
                       item.plan.events[0].onset == Rational(0) &&
                       item.plan.events[1].onset == timing.start.duration &&
                       item.plan.events[2].onset ==
                           timing.start.duration + timing.rise.duration &&
                       item.plan.events[3].onset ==
                           timing.start.duration + timing.rise.duration *
                               Rational(2) &&
                       item.plan.events[4].onset ==
                           timing.start.duration + timing.rise.duration *
                               Rational(2) + timing.fall.duration &&
                       item.plan.end() == timing.start.duration +
                           timing.rise.duration * Rational(2) +
                           timing.fall.duration * Rational(2);
            }),
        "Nahawand timing policy was not applied");
}
