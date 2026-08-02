#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Bayati.hpp"

#include <algorithm>

void test::bayati_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_bayati();
    require(scaffold.has_value(), scaffold.error_or("Bayati scaffold failed"));
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
        17,
        scaffold->generation.choice,
        scaffold->generation.production,
        scaffold->generation.projection,
        scaffold->generation.schema,
        {},
        limits);
    require(result.has_value(), result ? "" : result.error().message);
    require(
        result->legal.size() == 4 && result->rejected.empty(),
        "all four provisional Bayati routes must be legal");
    const auto stay = std::ranges::find_if(
        result->legal,
        [](const auto& item) { return item.plan.events.size() == 1; });
    require(stay != result->legal.end(), "Bayati stay route is missing");
    const auto journeys = std::ranges::count_if(
        result->legal,
        [](const auto& item) { return item.plan.events.size() == 5; });
    require(journeys == 3, "Bayati upper-jins routes are incomplete");
    require(
        std::ranges::all_of(
            result->legal,
            [](const auto& item) {
                return item.plan.well_formed() &&
                       !item.state.phrase.active &&
                       !item.state.gesture.active &&
                       !item.state.center.stack.empty() &&
                       item.state.cell.owners.size() ==
                           item.plan.events.size() &&
                       item.state.cell.occurrences.size() >= 1 &&
                       item.state.phrase.completed.size() >= 1;
            }),
        "Bayati routes did not produce complete timed plans");
    require(
        std::ranges::all_of(
            result->legal,
            [](const auto& item) {
                if (item.plan.events.size() == 1) {
                    return item.plan.events.front().duration ==
                               Rational(3, 2) &&
                           item.plan.events.front().intensity ==
                               Rational(3, 4) &&
                           item.plan.events.front().articulation ==
                               performance::Articulation::Neutral;
                }
                return item.plan.events.size() == 5 &&
                       item.plan.events[0].onset == Rational(0) &&
                       item.plan.events[1].onset == Rational(3, 2) &&
                       item.plan.events[2].onset == Rational(9, 4) &&
                       item.plan.events[3].onset == Rational(3) &&
                       item.plan.events[4].onset == Rational(7, 2) &&
                       item.plan.end() == Rational(4) &&
                       item.plan.events[1].articulation ==
                           performance::Articulation::Connected &&
                       item.plan.events[3].articulation ==
                           performance::Articulation::Detached;
            }),
        "external timing profile was not consumed by Bayati plans");
    const Identity question{
        "maqam.bayati", "phrase.question", "1"};
    const Identity response{
        "maqam.bayati", "phrase.response", "1"};
    require(
        std::ranges::all_of(
            result->legal,
            [&](const auto& item) {
                if (item.plan.events.size() == 1) {
                    return item.state.phrase.completed.size() == 1 &&
                           item.state.phrase.completed.front().function.identity ==
                               question;
                }
                const auto& spans = item.state.phrase.completed;
                return spans.size() == 2 &&
                       spans.front().function.identity == question &&
                       spans.back().function.identity == response &&
                       !spans.front().cadences.empty() &&
                       !spans.back().cadences.empty() &&
                       spans.front().cadences.back().strength == Rational(3, 4) &&
                       spans.back().cadences.back().strength == Rational(1);
            }),
        "Bayati routes did not retain provisional question-response boundaries");
    const Identity developCell{
        "maqam.bayati", "cell.develop", "1"};
    const Identity developFormula{
        "maqam.bayati", "formula.develop", "1"};
    const Identity variedFormula{
        "maqam.bayati", "formula.develop-variation", "1"};
    require(
        std::ranges::all_of(
            result->legal,
            [&](const auto& item) {
                if (item.plan.events.size() == 1) {
                    return true;
                }
                const auto& owners = item.state.cell.owners;
                const auto first = owners.at(sort::EventId{
                    item.plan.events[1].target.event.identity});
                const auto second = owners.at(sort::EventId{
                    item.plan.events[3].target.event.identity});
                return first.cell.identity == developCell &&
                       second.cell.identity == developCell &&
                       first.variation && second.variation &&
                       first.variation->identity == developFormula &&
                       second.variation->identity == variedFormula;
            }),
        "Bayati did not preserve a repeated cell with an explicit variation");
}
