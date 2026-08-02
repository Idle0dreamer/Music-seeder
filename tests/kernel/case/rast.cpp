#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Rast.hpp"

#include <algorithm>
#include <string>

void test::rast_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_rast();
    require(scaffold.has_value(), scaffold.error_or("Rast scaffold failed"));
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
        23,
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
            "Rast routes did not all complete: legal=" +
                std::to_string(result->legal.size()) + " " + candidate +
                " " + detail);
    }
    require(
        std::ranges::all_of(
            result->legal,
            [](const auto& item) {
                return item.plan.well_formed() &&
                       !item.state.phrase.active &&
                       !item.state.gesture.active &&
                       item.state.cell.owners.size() ==
                           item.plan.events.size() &&
                       item.state.phrase.completed.size() >= 1;
            }),
        "Rast produced an incomplete plan");
    require(
        std::ranges::all_of(
            result->legal,
            [](const auto& item) {
                if (item.plan.events.size() == 1) {
                    return item.plan.events.front().duration == Rational(3, 2);
                }
                return item.plan.events.size() == 5 &&
                       item.plan.events[0].onset == Rational(0) &&
                       item.plan.events[1].onset == Rational(3, 2) &&
                       item.plan.events[2].onset == Rational(9, 4) &&
                       item.plan.events[3].onset == Rational(3) &&
                       item.plan.events[4].onset == Rational(7, 2) &&
                       item.plan.end() == Rational(4);
            }),
        "external timing profile was not applied to Rast");
}
