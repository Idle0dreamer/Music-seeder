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
        define("allow.cadence", {fixture.cadence}, source),
        define("allow.tonicize", {fixture.jins.branch}, source),
        define("allow.modulate", {fixture.path.direct}, source),
        define(
            "allow.place",
            {fixture.role.root, fixture.role.ghammaz},
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

    const std::vector<profile::Patch> a{
        {
            profile::Patch::Action::Parameterize,
            "threshold.internal.emphasis",
            rule(Rational(3), "fixture:regional-a"),
        },
    };
    auto regionalA =
        profile::reconstruct("fixture.regional-a", {*shared}, a);
    if (!regionalA) {
        return std::unexpected(regionalA.error());
    }

    const std::vector<profile::Patch> b{
        {
            profile::Patch::Action::Forbid,
            "allow.modulate",
            profile::Rule{false, {"fixture:regional-b"}},
        },
    };
    auto regionalB =
        profile::reconstruct("fixture.regional-b", {*shared}, b);
    if (!regionalB) {
        return std::unexpected(regionalB.error());
    }
    return Profiles{
        *shared,
        {*regionalA, *regionalB},
    };
}

} // namespace mq::kernel::fixture::detail
