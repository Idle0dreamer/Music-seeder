#include "Test.hpp"

#include "mq/kernel/Evaluator.hpp"
#include "mq/kernel/Fixture.hpp"

#include <vector>

void evaluatorTests() {
    using namespace mq::kernel;
    const auto fixtureResult = makeFixture();
    require(fixtureResult.has_value(), fixtureResult.error_or("fixture failed"));
    const auto& fixture = *fixtureResult;

    const std::vector<Operation> premature{
        Anchor{fixture.rootCenter},
        Enter{fixture.rootJins},
        Tonicize{fixture.branchJins, TonicizationLevel::Internal},
    };
    Evaluator sharedEvaluator(fixture.shared);
    const auto rejected = sharedEvaluator.run({}, premature);
    require(!rejected, "one entered jins incorrectly proved tonicization");
    require(
        rejected.error().rule == "threshold.internal.dwell",
        "tonicization failure did not identify missing evidence");

    const std::vector<Operation> established{
        Anchor{fixture.rootCenter},
        Enter{fixture.rootJins},
        Emphasize{fixture.ghammazRole, Rational(2)},
        Dwell{fixture.ghammazRole, Rational(2)},
        Emit{fixture.cell},
        Cadence{fixture.cadence, Rational(1)},
        Tonicize{fixture.branchJins, TonicizationLevel::Internal},
        Modulate{
            fixture.path,
            fixture.branchCenter,
            TonicizationLevel::Internal,
        },
        Return{fixture.rootCenter},
    };
    const auto accepted = sharedEvaluator.run({}, established);
    require(accepted.has_value(), accepted.error().message);
    require(
        accepted->centerStack.size() == 1 &&
            accepted->centerStack.back() == fixture.rootCenter,
        "return did not restore the established center");

    Evaluator regionalAEvaluator(fixture.regionalA);
    const auto regionalARejected =
        regionalAEvaluator.run({}, established);
    require(
        !regionalARejected &&
            regionalARejected.error().rule ==
                "threshold.internal.emphasis",
        "regional evidence threshold was not reconstructed");

    auto stronger = established;
    std::get<Emphasize>(stronger[2]).amount = Rational(3);
    require(
        regionalAEvaluator.run({}, stronger).has_value(),
        "regional evidence threshold rejected sufficient evidence");

    Evaluator regionalBEvaluator(fixture.regionalB);
    const auto regionalBRejected =
        regionalBEvaluator.run({}, stronger);
    require(
        !regionalBRejected &&
            regionalBRejected.error().rule == "allow.modulate",
        "regional path prohibition was not enforced");
}

