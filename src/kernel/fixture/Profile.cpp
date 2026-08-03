#include "Internal.hpp"

namespace mq::kernel::fixture::detail {
namespace {

profile::Rule rule(profile::Domain values, std::string provenance) {
    return profile::Rule{std::move(values), {std::move(provenance)}};
}

profile::Rule rule(Rational value, std::string provenance) {
    return profile::Rule{value, {std::move(provenance)}};
}

profile::Patch define(
    std::string key,
    profile::Domain values,
    const std::string& source) {
    return {
        profile::Patch::Action::Define,
        std::move(key),
        rule(std::move(values), source),
    };
}

profile::Patch define(
    std::string key,
    Rational value,
    const std::string& source) {
    return {
        profile::Patch::Action::Define,
        std::move(key),
        rule(value, source),
    };
}

} // namespace

std::expected<Profiles, std::string> profiles(
    const Set& fixture) {
    const std::string source = "fixture:shared-semantics";
    const std::vector<profile::Patch> patches{
        define("allow.anchor", {fixture.center.root}, source),
        define("allow.enter", {fixture.jins.root, fixture.jins.branch}, source),
        define(
            "allow.emphasize",
            {fixture.role.root, fixture.role.ghammaz},
            source),
        define(
            "allow.dwell",
            {fixture.role.root, fixture.role.ghammaz},
            source),
        define("allow.emit", {fixture.cell}, source),
        define(
            "allow.variation",
            {Identity{"test.formula", "base", "1"},
             Identity{"test.formula", "variation", "1"}},
            source),
        define("allow.cadence", {fixture.cadence}, source),
        define("allow.tonicize", {fixture.jins.branch}, source),
        define("allow.modulate", {fixture.path.direct}, source),
        define(
            "allow.place",
            {
                fixture.role.root,
                fixture.role.ghammaz,
                fixture.role.baggage,
            },
            source),
        define(
            "allow.register",
            {fixture.region.root, fixture.region.upper},
            source),
        define("allow.baggage", {fixture.baggage}, source),
        define(
            "allow.gesture",
            {fixture.gesture.ascent, fixture.gesture.descent},
            source),
        define(
            "allow.phrase.function",
            {fixture.phrase.function},
            source),
        define("threshold.internal.emphasis", Rational(2), source),
        define("threshold.internal.dwell", Rational(2), source),
        define("threshold.internal.cell", Rational(1), source),
        define("threshold.internal.cadence", Rational(1), source),
    };
    auto shared = profile::reconstruct("fixture.shared", {}, patches);
    if (!shared) {
        return std::unexpected(shared.error());
    }

    const struct {
        std::vector<profile::Patch> a;
        std::vector<profile::Patch> b;
    } regional{
        {{
            profile::Patch::Action::Parameterize,
            "threshold.internal.emphasis",
            rule(Rational(3), "fixture:regional-a"),
        }},
        {
            {
                profile::Patch::Action::Forbid,
                "allow.modulate",
                profile::Rule{false, {"fixture:regional-b"}},
            },
            {
                profile::Patch::Action::Forbid,
                "allow.baggage",
                profile::Rule{false, {"fixture:regional-b"}},
            },
        },
    };
    auto a =
        profile::reconstruct("fixture.regional-a", {*shared}, regional.a);
    if (!a) {
        return std::unexpected(a.error());
    }

    auto b =
        profile::reconstruct("fixture.regional-b", {*shared}, regional.b);
    if (!b) {
        return std::unexpected(b.error());
    }
    return Profiles{
        *shared,
        {*a, *b},
    };
}

} // namespace mq::kernel::fixture::detail
