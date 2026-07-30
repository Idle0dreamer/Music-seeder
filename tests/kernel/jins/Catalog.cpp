#include "../Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/Set.hpp"

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.jins.catalog", std::move(name), "1"};
}

} // namespace

void test::jins::catalog() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const auto* root = fixture.catalog.find(fixture.jins.root);
    const auto* branch = fixture.catalog.find(fixture.jins.branch);
    require(
        fixture.catalog.size() == 2 && root != nullptr && branch != nullptr,
        "neutral jins catalog lost an admitted descriptor");

    mq::kernel::jins::Catalog forward;
    mq::kernel::jins::Catalog reverse;
    require(
        forward.add(*root).has_value() &&
            forward.add(*branch).has_value() &&
            reverse.add(*branch).has_value() &&
            reverse.add(*root).has_value() &&
            *forward.find(fixture.jins.root) ==
                *reverse.find(fixture.jins.root) &&
            *forward.find(fixture.jins.branch) ==
                *reverse.find(fixture.jins.branch),
        "catalog insertion order changed descriptor meaning");

    mq::kernel::jins::Catalog duplicate;
    require(
        duplicate.add(*root).has_value() && !duplicate.add(*root),
        "catalog accepted a repeated descriptor identity");

    auto malformed = *root;
    malformed.tonic = id("outside-role");
    mq::kernel::jins::Catalog invalidCore;
    require(
        !invalidCore.add(std::move(malformed)),
        "catalog accepted a tonic outside the descriptor role domain");

    auto crossed = *root;
    crossed.gestures.begin()->second.steps.back().baggage =
        id("missing-baggage");
    mq::kernel::jins::Catalog invalidReference;
    require(
        !invalidReference.add(std::move(crossed)),
        "catalog accepted a dangling gesture baggage reference");

    const std::vector<operation::Any> enter{
        operation::Enter{fixture.jins.root},
    };
    const auto noCatalog =
        eval::Evaluator(fixture.profile.shared).run({}, enter);
    require(
        !noCatalog && noCatalog.error().rule == "jins.catalog",
        "jins entry silently fell back without a descriptor catalog");

    mq::kernel::jins::Catalog empty;
    const auto unknown =
        eval::Evaluator(fixture.profile.shared, empty).run({}, enter);
    require(
        !unknown && unknown.error().rule == "jins.descriptor",
        "jins entry silently accepted an absent descriptor");
}
