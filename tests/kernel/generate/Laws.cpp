#include "../Test.hpp"

#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/generate/Engine.hpp"

#include <algorithm>

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.generate.law", std::move(name), "1"};
}

} // namespace

void test::generate::laws() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& set = *made;
    const auto generated = fixture::generation::make(set);
    require(generated.has_value(), generated.error_or("generation failed"));
    const auto& value = *generated;
    const eval::Context context{
        .jins = {&set.catalog},
        .path = {&set.path.graph},
        .sayr = {&set.sayr.plan},
    };
    const mq::kernel::generate::Engine engine(
        set.profile.shared,
        context);
    std::vector candidates{value.program.stay, value.program.travel};
    const auto result = engine.run(
        91,
        value.choice,
        candidates,
        value.projection,
        value.schema);
    require(result.has_value(), result ? "" : result.error().message);
    const auto stayed = std::ranges::find(
        result->legal,
        value.program.stay.identity,
        &mq::kernel::generate::Outcome::candidate);
    const auto traveled = std::ranges::find(
        result->legal,
        value.program.travel.identity,
        &mq::kernel::generate::Outcome::candidate);
    require(
        result->legal.size() == 2 &&
            result->rejected.empty() &&
            stayed != result->legal.end() &&
            stayed->plan.targets.size() == 1 &&
            stayed->plan.targets.front().center == pitch::Expression{} &&
            stayed->state.gesture.completed.empty() &&
            stayed->state.sayr.completed.contains(
                set.sayr.obligation.settle) &&
            traveled != result->legal.end() &&
            traveled->plan.targets.size() == 5 &&
            traveled->plan.targets[0].center == pitch::Expression{} &&
            traveled->plan.targets[1].center ==
                pitch::Expression::ratio(4, 3) &&
            traveled->plan.targets[2].center ==
                pitch::Expression::ratio(3, 2) &&
            traveled->plan.targets[3].center ==
                pitch::Expression::ratio(4, 3) &&
            traveled->plan.targets[4].center == pitch::Expression{} &&
            traveled->state.gesture.completed.size() == 2 &&
            traveled->state.gesture.completed.front().first ==
                traveled->plan.targets.front().event.identity &&
            traveled->state.gesture.completed.front().last ==
                traveled->plan.targets[2].event.identity &&
            traveled->state.gesture.completed.back().first ==
                traveled->plan.targets[3].event.identity &&
            traveled->state.gesture.completed.back().last ==
                traveled->plan.targets.back().event.identity &&
            traveled->state.sayr.completed.contains(
                set.sayr.obligation.restore) &&
            traveled->state.center.stack.back() == set.center.root &&
            traveled->state.jins.active == set.jins.root &&
            std::ranges::all_of(
                result->legal,
                [](const auto& outcome) {
                    return !outcome.state.phrase.active &&
                           !outcome.state.gesture.active &&
                           !outcome.state.phrase.completed.empty();
                }),
        "generator did not evaluate all complete legal candidates");

    std::ranges::reverse(candidates);
    const auto reordered = engine.run(
        91,
        value.choice,
        candidates,
        value.projection,
        value.schema);
    require(
        reordered && reordered->selected == result->selected,
        "candidate storage order changed seeded generation");

    auto incomplete = value.program.stay;
    incomplete.identity = id("incomplete");
    incomplete.stages.front().actions.pop_back();
    const std::vector unfinished{incomplete};
    const auto unfulfilled = engine.run(
        0,
        value.choice,
        unfinished,
        value.projection,
        value.schema);
    require(
        !unfulfilled &&
            unfulfilled.error().code ==
                mq::kernel::generate::Error::Code::NoLegal &&
            unfulfilled.error().rejected.size() == 1 &&
            unfulfilled.error().rejected.front().evaluation &&
            unfulfilled.error().rejected.front().evaluation->rule ==
                "sayr.route",
        "candidate completed without a declared sayr route");

    auto malformed = value.program.stay;
    malformed.identity = id("malformed");
    malformed.stages.front().actions.push_back(operation::Place{
        id("extra"),
        set.role.root,
        motion::Direction::Same,
        set.region.root,
        std::nullopt,
    });
    const std::vector invalid{malformed};
    const auto rejected = engine.run(
        0,
        value.choice,
        invalid,
        value.projection,
        value.schema);
    require(
        !rejected &&
            rejected.error().code ==
                mq::kernel::generate::Error::Code::Input,
        "stage accepted more than one structural event");

    auto limited = mq::kernel::generate::Limits{};
    limited.candidates = 1;
    const auto oversized = engine.run(
        0,
        value.choice,
        candidates,
        value.projection,
        value.schema,
        {},
        limited);
    require(
        !oversized &&
            oversized.error().code ==
                mq::kernel::generate::Error::Code::Count,
        "generator candidate budget was not enforced");
}
