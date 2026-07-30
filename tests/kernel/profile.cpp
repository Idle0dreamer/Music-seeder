#include "Test.hpp"

#include "mq/kernel/fixture/Set.hpp"

void test::profile() {
    using namespace mq::kernel;
    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;

    require(
        fixture.profile.shared.allows(
            "allow.modulate",
            fixture.path.direct),
        "shared profile lost permitted path");
    require(
        !fixture.profile.regional.b.allows(
            "allow.modulate",
            fixture.path.direct),
        "regional prohibition did not reconstruct");
    require(
        *fixture.profile.regional.a.parameter("threshold.internal.emphasis") ==
            Rational(3),
        "regional parameterization did not reconstruct");

    const Identity extra{"fixture.path", "unsupported-extra", "1"};
    const profile::Patch widening{
        profile::Patch::Action::Refine,
        "allow.modulate",
        profile::Rule{
            profile::Domain{fixture.path.direct, extra},
            {"test:illegal-widen"},
        },
    };
    require(
        !profile::reconstruct(
            "invalid.refine",
            {fixture.profile.shared},
            {widening}),
        "Refine illegally widened a domain");

    const profile::Patch exception{
        profile::Patch::Action::AddException,
        "allow.modulate",
        profile::Rule{profile::Domain{extra}, {"test:sourced-exception"}},
    };
    const auto widened =
        profile::reconstruct(
            "valid.exception",
            {fixture.profile.shared},
            {exception});
    require(widened.has_value(), widened.error_or("exception failed"));
    require(
        widened->allows("allow.modulate", extra),
        "explicit exception did not widen its named domain");

    const profile::Patch threshold{
        profile::Patch::Action::Parameterize,
        "threshold.internal.emphasis",
        profile::Rule{Rational(7), {"test:other-parent"}},
    };
    const auto parent = profile::reconstruct(
        "fixture.other-parent",
        {fixture.profile.shared},
        {threshold});
    require(parent.has_value(), parent.error_or("parent failed"));
    require(
        !profile::reconstruct(
            "fixture.conflicted",
            {fixture.profile.regional.a, *parent},
            {}),
        "multiple-parent conflict was silently ordered");

    const profile::Patch resolution{
        profile::Patch::Action::Resolve,
        "threshold.internal.emphasis",
        profile::Rule{Rational(5), {"test:explicit-resolution"}},
    };
    const auto resolved = profile::reconstruct(
        "fixture.resolved",
        {fixture.profile.regional.a, *parent},
        {resolution});
    require(resolved.has_value(), resolved.error_or("resolution failed"));
    require(
        *resolved->parameter("threshold.internal.emphasis") == Rational(5),
        "explicit conflict resolution was not applied");
}
