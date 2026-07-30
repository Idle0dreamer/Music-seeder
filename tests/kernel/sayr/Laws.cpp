#include "../Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/generation/Set.hpp"

#include <algorithm>

namespace ks = mq::kernel::sayr;

void test::sayr::laws() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const auto generated = fixture::generation::make(fixture);
    require(generated.has_value(), generated.error_or("generation failed"));
    const eval::Context context{
        .jins = {&fixture.catalog},
        .path = {&fixture.path.graph},
        .sayr = {&fixture.sayr.plan},
    };
    const eval::Evaluator evaluator(fixture.profile.shared, context);
    const operation::sayr::Fulfill establish{
        fixture.sayr.obligation.establish,
    };
    const operation::sayr::Fulfill settle{
        fixture.sayr.obligation.settle,
    };

    const auto noPlan = eval::Evaluator(
        fixture.profile.shared,
        fixture.catalog).run(
            {},
            std::vector<operation::Any>{establish});
    require(
        !noPlan && noPlan.error().rule == "sayr.plan",
        "sayr fulfillment silently ran without a plan");

    const auto unordered = evaluator.run(
        {},
        std::vector<operation::Any>{settle});
    require(
        !unordered && unordered.error().rule == "sayr.order",
        "sayr fulfillment skipped an incomplete predecessor");

    const auto missing = evaluator.run(
        {},
        std::vector<operation::Any>{establish});
    require(
        !missing &&
            missing.error().rule == "sayr.need" &&
            missing.error().message.find("establish") !=
                std::string::npos,
        "sayr failure lost its exact unsatisfied need");

    const auto& actions =
        generated->program.stay.stages.front().actions;
    const auto completed = evaluator.run({}, actions);
    require(
        completed &&
            completed->sayr.history.size() == 2 &&
            fixture.sayr.plan.accepts(completed->sayr.history) &&
            completed->sayr.history.front().obligation ==
                fixture.sayr.obligation.establish &&
            completed->sayr.history.back().obligation ==
                fixture.sayr.obligation.settle &&
            completed->trace.events.back().operation ==
                "Sayr.Fulfill",
        "checked sayr fulfillment lost order or route completion");

    const auto repeated = evaluator.run(
        *completed,
        std::vector<operation::Any>{settle});
    require(
        !repeated && repeated.error().rule == "sayr.repeat",
        "sayr obligation completed twice");

    auto corrupt = *completed;
    corrupt.sayr.completed.clear();
    const auto malformed = evaluator.run(
        std::move(corrupt),
        std::vector<operation::Any>{settle});
    require(
        !malformed && malformed.error().rule == "sayr.state",
        "sayr fulfillment accepted inconsistent completion state");

    const auto& proof = completed->sayr.history.front().proofs;
    require(
        std::ranges::any_of(
            proof,
            [](const auto& item) {
                return std::holds_alternative<ks::proof::Phrase>(
                    item.value);
            }) &&
            std::ranges::any_of(
                proof,
                [&](const auto& item) {
                    const auto* role =
                        std::get_if<ks::proof::Role>(&item.value);
                    return role != nullptr &&
                           role->role == fixture.role.root;
                }),
        "sayr completion lost its exact phrase or event proofs");
}
