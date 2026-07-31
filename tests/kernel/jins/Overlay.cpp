#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/jins/Overlay.hpp"

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.jins.overlay", std::move(name), "1"};
}

} // namespace

void test::jins::overlay() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const auto* root = fixture.catalog.find(fixture.jins.root);
    require(root != nullptr, "root descriptor not found");
    auto base = *root;

    // Apply some valid overlays
    std::vector<mq::kernel::jins::Overlay> overlays;
    overlays.push_back(mq::kernel::jins::Overlay{
        mq::kernel::jins::Overlay::DefineRole{id("new-role")}, {"add role"}});
    overlays.push_back(mq::kernel::jins::Overlay{
        mq::kernel::jins::Overlay::ReplaceTonic{id("new-role")}, {"set tonic"}});

    auto result = mq::kernel::jins::reconstruct(base, overlays);
    require(result.has_value(), result.error_or("overlay application failed"));
    require(result->roles.contains(id("new-role")), "role was not added");
    require(result->tonic == id("new-role"), "tonic was not replaced");

    // Test formula overlays
    const auto gestureId = base.gestures.begin()->first;
    std::vector<mq::kernel::jins::Overlay> formula_overlays;
    formula_overlays.push_back(mq::kernel::jins::Overlay{
        mq::kernel::jins::Overlay::DefineEntry{gestureId}, {"add entry"}});
    auto formula_result = mq::kernel::jins::reconstruct(base, formula_overlays);
    require(formula_result.has_value(), formula_result.error_or("formula overlay failed"));
    require(formula_result->entry.contains(gestureId), "entry formula not added");

    std::vector<mq::kernel::jins::Overlay> bad_formula_overlays;
    bad_formula_overlays.push_back(mq::kernel::jins::Overlay{
        mq::kernel::jins::Overlay::DefineEntry{id("unknown-gesture")}, {"bad entry"}});
    auto bad_formula_result = mq::kernel::jins::reconstruct(base, bad_formula_overlays);
    require(!bad_formula_result, "invalid formula overlay was accepted");

    // Invalid overlay that violates invariants
    std::vector<mq::kernel::jins::Overlay> bad_overlays;
    bad_overlays.push_back(mq::kernel::jins::Overlay{
        mq::kernel::jins::Overlay::ReplaceTonic{id("unknown-role")}, {"bad tonic"}});
    auto bad_result = mq::kernel::jins::reconstruct(base, bad_overlays);
    require(!bad_result, "invalid overlay was accepted");
}
