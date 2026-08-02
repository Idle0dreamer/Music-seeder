#include "../Test.hpp"

#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/jins/Overlay.hpp"

namespace {



} // namespace

void test::generate::overlay() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    auto set = *made;
    const auto generated = fixture::generation::make(set);
    require(generated.has_value(), generated.error_or("generation failed"));
    const auto& value = *generated;
    const mq::kernel::generate::Limits limits{
        .timing = test::timing_profile()};

    const std::vector candidates{
        value.program.stay,
        value.program.travel,
    };

    // First run with the base catalog
    const eval::Context base_context{
        .jins = {&set.catalog},
        .path = {&set.path.graph},
        .sayr = {&set.sayr.plan},
        .grammar = {},
    };
    const mq::kernel::generate::Engine base_engine(
        set.profile.shared,
        base_context);

    std::optional<std::uint64_t> seed;
    for (std::uint64_t candidate = 0; candidate < 512; ++candidate) {
        const auto result = base_engine.run(
            candidate,
            value.choice,
            candidates,
            value.projection,
            value.schema,
            {},
            limits);
        if (result && result->selected == value.program.travel.identity) {
            seed = candidate;
            break;
        }
    }
    require(seed.has_value(), "no seed selected the travel candidate");

    // Create an overlay that restricts baggage.
    // The travel candidate uses baggage, so removing it from the descriptor
    // should cause the candidate to fail evaluation.
    const auto* root_desc = set.catalog.find(set.jins.root);
    require(root_desc != nullptr, "root descriptor not found");

    std::vector<mq::kernel::jins::Overlay> overlays;
    overlays.push_back(mq::kernel::jins::Overlay{
        mq::kernel::jins::Overlay::RemoveBaggage{set.baggage},
        {"remove baggage"}});
    overlays.push_back(mq::kernel::jins::Overlay{
        mq::kernel::jins::Overlay::RemoveGesture{set.gesture.ascent},
        {"remove gesture"}});

    const auto modified_desc = mq::kernel::jins::reconstruct(*root_desc, overlays);
    require(modified_desc.has_value(), modified_desc.error_or("overlay failed"));

    // Build a new catalog with the modified descriptor
    mq::kernel::jins::Catalog modified_catalog;
    require(modified_catalog.add(*modified_desc).has_value(), "failed to add modified descriptor");
    // We also need to add the other descriptors if travel uses them.
    const auto* branch_desc = set.catalog.find(set.jins.branch);
    if (branch_desc) {
        require(modified_catalog.add(*branch_desc).has_value(), "failed to add branch descriptor");
    }

    const eval::Context modified_context{
        .jins = {&modified_catalog},
        .path = {&set.path.graph},
        .sayr = {&set.sayr.plan},
        .grammar = {},
    };

    const mq::kernel::generate::Engine modified_engine(
        set.profile.shared,
        modified_context);

    const auto modified_result = modified_engine.run(
        *seed,
        value.choice,
        candidates,
        value.projection,
        value.schema,
        {},
        limits);

    require(
        modified_result &&
            modified_result->selected == value.program.stay.identity &&
            modified_result->legal.size() == 1 &&
            modified_result->rejected.size() == 1 &&
            modified_result->rejected.front().candidate ==
                value.program.travel.identity &&
            modified_result->rejected.front().evaluation &&
            modified_result->rejected.front().evaluation->rule == "jins.gesture",
        "descriptor overlay did not alter executable generation appropriately");
}
