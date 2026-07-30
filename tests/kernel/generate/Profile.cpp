#include "../Test.hpp"

#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/generate/Engine.hpp"

void test::generate::profile() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& set = *made;
    const auto generated = fixture::generation::make(set);
    require(generated.has_value(), generated.error_or("generation failed"));
    const auto& value = *generated;
    const std::vector candidates{
        value.program.stay,
        value.program.travel,
    };
    const mq::kernel::generate::Engine regionalA(
        set.profile.regional.a,
        set.path.graph);
    const mq::kernel::generate::Engine regionalB(
        set.profile.regional.b,
        set.path.graph);

    std::optional<std::uint64_t> seed;
    for (std::uint64_t candidate = 0; candidate < 512; ++candidate) {
        const auto result = regionalA.run(
            candidate,
            value.choice,
            candidates,
            value.projection,
            value.schema);
        if (result &&
            result->selected == value.program.travel.identity) {
            seed = candidate;
            break;
        }
    }
    require(seed.has_value(), "no seed selected the legal travel candidate");

    const auto a = regionalA.run(
        *seed,
        value.choice,
        candidates,
        value.projection,
        value.schema);
    const auto b = regionalB.run(
        *seed,
        value.choice,
        candidates,
        value.projection,
        value.schema);
    require(
        a &&
            a->selected == value.program.travel.identity &&
            a->legal.size() == 2,
        "regional A did not retain its complete legal generation set");
    require(
        b &&
            b->selected == value.program.stay.identity &&
            b->legal.size() == 1 &&
            b->rejected.size() == 1 &&
            b->rejected.front().candidate ==
                value.program.travel.identity &&
            b->rejected.front().evaluation &&
            b->rejected.front().evaluation->rule == "allow.modulate",
        "regional prohibition did not alter executable generation");

    const std::vector onlyTravel{value.program.travel};
    const auto none = regionalB.run(
        *seed,
        value.choice,
        onlyTravel,
        value.projection,
        value.schema);
    require(
        !none &&
            none.error().code ==
                mq::kernel::generate::Error::Code::NoLegal &&
            none.error().rejected.size() == 1,
        "no-legal generation lost its rejection evidence");
}
