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
    const mq::kernel::generate::Engine engine(
        set.profile.shared,
        set.path.graph);
    std::vector candidates{value.program.stay, value.program.travel};
    const auto result = engine.run(
        91,
        value.choice,
        candidates,
        value.projection,
        value.schema);
    require(
        result &&
            result->legal.size() == 2 &&
            result->rejected.empty() &&
            std::ranges::all_of(
                result->legal,
                [](const auto& outcome) {
                    return outcome.plan.targets.size() == 1 &&
                           !outcome.state.phrase.active &&
                           outcome.state.phrase.completed.size() == 1;
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

    auto malformed = value.program.stay;
    malformed.identity = id("malformed");
    malformed.stages.front().actions.push_back(operation::Place{
        id("extra"),
        set.role.root,
        motion::Direction::Same,
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
