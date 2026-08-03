#include "../Test.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>

void test::bayati_case() {
    using namespace mq::kernel;
    const auto catalog = maqam::Catalog::declared();
    auto scaffold = catalog.build_executable("bayati");
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
    if (result->legal.size() != 4 || !result->rejected.empty()) {
        std::string message =
            "all four provisional Bayati routes must be legal; legal=" +
            std::to_string(result->legal.size()) +
            " rejected=" + std::to_string(result->rejected.size());
        for (const auto& rejected : result->rejected) {
            message += " [" + rejected.candidate.str() + ":" +
                       rejected.message + "]";
        }
        for (const auto& diagnostic : result->derivation) {
            message += " {" + diagnostic.term.str() + ":" +
                       diagnostic.message + "}";
        }
        require(false, message);
    }
    const auto stay = std::ranges::find_if(
        result->legal,
        [](const auto& item) { return item.plan.events.size() == 1; });
    require(stay != result->legal.end(), "Bayati stay route is missing");
    const auto journeys = std::ranges::count_if(
        result->legal,
        [](const auto& item) { return item.plan.events.size() == 6; });
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
                const auto timing = test::timing_profile();
                if (item.plan.events.size() == 1) {
                    return item.plan.events.front().duration ==
                               timing.start.duration &&
                           item.plan.events.front().intensity ==
                               timing.start.intensity &&
                           item.plan.events.front().articulation ==
                               timing.start.articulation &&
                           item.plan.events.front().release &&
                           !item.plan.pauses.empty();
                }
                return item.plan.events.size() == 6 &&
                       item.plan.events[0].onset == Rational(0) &&
                       item.plan.events[0].duration == timing.start.duration &&
                       item.plan.events[1].duration == timing.rise.duration &&
                       std::ranges::all_of(
                           item.plan.events,
                           [](const auto& event) { return event.release.has_value(); }) &&
                       !item.plan.pauses.empty() &&
                       item.plan.events[1].articulation ==
                           timing.rise.articulation &&
                           item.plan.events[4].articulation ==
                           timing.fall.articulation;
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
                    item.plan.events[4].target.event.identity});
                const auto& target = item.plan.events[1].target;
                return first.cell.identity == developCell &&
                       second.cell.identity == developCell &&
                       target.cell && target.cell->identity == developCell &&
                       target.formula &&
                       target.formula->identity == developFormula &&
                       target.motif &&
                       target.motif->identity ==
                           Identity{"maqam.bayati", "motif.develop", "1"} &&
                       first.formula &&
                       first.formula->identity == developFormula &&
                       second.formula &&
                       second.formula->identity == developFormula &&
                       first.motif && second.motif &&
                       first.motif->identity ==
                           Identity{"maqam.bayati", "motif.develop", "1"} &&
                       first.motif == second.motif &&
                       !first.variation && second.variation &&
                       second.variation->identity == variedFormula &&
                       item.plan.events[4].ornament &&
                       item.plan.events[4].ornament->kind ==
                           performance::OrnamentKind::Oscillation;
            }),
        "Bayati did not preserve a repeated cell with an explicit variation");
}
