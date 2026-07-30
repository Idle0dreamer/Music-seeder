#include "Test.hpp"

#include "mq/kernel/Fixture.hpp"

void profileTests() {
    using namespace mq::kernel;
    const auto fixtureResult = makeFixture();
    require(fixtureResult.has_value(), fixtureResult.error_or("fixture failed"));
    const auto& fixture = *fixtureResult;

    require(
        fixture.shared.allows("allow.modulate", fixture.path),
        "shared profile lost permitted path");
    require(
        !fixture.regionalB.allows("allow.modulate", fixture.path),
        "regional prohibition did not reconstruct");
    require(
        *fixture.regionalA.parameter("threshold.internal.emphasis") ==
            Rational(3),
        "regional parameterization did not reconstruct");

    const Identity extra{"fixture.path", "unsupported-extra", "1"};
    const Patch widening{
        PatchAction::Refine,
        "allow.modulate",
        Rule{Domain{fixture.path, extra}, {"test:illegal-widen"}},
    };
    require(
        !reconstruct("invalid.refine", {fixture.shared}, {widening}),
        "Refine illegally widened a domain");

    const Patch exception{
        PatchAction::AddException,
        "allow.modulate",
        Rule{Domain{extra}, {"test:sourced-exception"}},
    };
    const auto widened =
        reconstruct("valid.exception", {fixture.shared}, {exception});
    require(widened.has_value(), widened.error_or("exception failed"));
    require(
        widened->allows("allow.modulate", extra),
        "explicit exception did not widen its named domain");

    const Patch differentThreshold{
        PatchAction::Parameterize,
        "threshold.internal.emphasis",
        Rule{Rational(7), {"test:other-parent"}},
    };
    const auto otherParent = reconstruct(
        "fixture.other-parent",
        {fixture.shared},
        {differentThreshold});
    require(otherParent.has_value(), otherParent.error_or("parent failed"));
    require(
        !reconstruct(
            "fixture.conflicted",
            {fixture.regionalA, *otherParent},
            {}),
        "multiple-parent conflict was silently ordered");

    const Patch resolution{
        PatchAction::Resolve,
        "threshold.internal.emphasis",
        Rule{Rational(5), {"test:explicit-resolution"}},
    };
    const auto resolved = reconstruct(
        "fixture.resolved",
        {fixture.regionalA, *otherParent},
        {resolution});
    require(resolved.has_value(), resolved.error_or("resolution failed"));
    require(
        *resolved->parameter("threshold.internal.emphasis") == Rational(5),
        "explicit conflict resolution was not applied");
}

