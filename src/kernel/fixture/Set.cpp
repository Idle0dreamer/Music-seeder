#include "mq/kernel/fixture/Set.hpp"

namespace mq::kernel::fixture {
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
    return profile::Patch{
        profile::Patch::Action::Define,
        std::move(key),
        rule(std::move(values), source),
    };
}

profile::Patch define(
    std::string key,
    Rational value,
    const std::string& source) {
    return profile::Patch{
        profile::Patch::Action::Define,
        std::move(key),
        rule(value, source),
    };
}

} // namespace

std::expected<Set, std::string> make() {
    Set fixture{
        .center = {
            Identity{"fixture.center", "root", "1"},
            Identity{"fixture.center", "branch", "1"},
        },
        .jins = {
            Identity{"fixture.jins", "root", "1"},
            Identity{"fixture.jins", "branch", "1"},
        },
        .role = {
            Identity{"fixture.role", "root", "1"},
            Identity{"fixture.role", "ghammaz", "1"},
        },
        .cell = Identity{"fixture.cell", "establish", "1"},
        .cadence = Identity{"fixture.cadence", "local", "1"},
        .path = {
            Identity{"fixture.path", "root-to-branch", "1"},
            {},
        },
        .profile = {},
    };
    const auto added = fixture.path.graph.add(path::Rule{
        fixture.path.direct,
        fixture.center.root,
        fixture.center.branch,
        tonicization::Level::Internal,
        {},
        "fixture:neutral-path",
    });
    if (!added) {
        return std::unexpected(added.error());
    }

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
            "threshold.internal.emphasis",
            Rational(2),
            source),
        define(
            "threshold.internal.dwell",
            Rational(2),
            source),
        define(
            "threshold.internal.cell",
            Rational(1),
            source),
        define(
            "threshold.internal.cadence",
            Rational(1),
            source),
    };

    auto shared = profile::reconstruct("fixture.shared", {}, patches);
    if (!shared) {
        return std::unexpected(shared.error());
    }
    fixture.profile.shared = *shared;

    {
        const std::vector<profile::Patch> regional{
            profile::Patch{
                profile::Patch::Action::Parameterize,
                "threshold.internal.emphasis",
                rule(Rational(3), "fixture:regional-a"),
            },
        };
        auto result =
            profile::reconstruct("fixture.regional-a", {*shared}, regional);
        if (!result) {
            return std::unexpected(result.error());
        }
        fixture.profile.regional.a = *result;
    }

    {
        const std::vector<profile::Patch> regional{
            profile::Patch{
                profile::Patch::Action::Forbid,
                "allow.modulate",
                profile::Rule{false, {"fixture:regional-b"}},
            },
        };
        auto result =
            profile::reconstruct("fixture.regional-b", {*shared}, regional);
        if (!result) {
            return std::unexpected(result.error());
        }
        fixture.profile.regional.b = *result;
    }

    return fixture;
}

} // namespace mq::kernel::fixture
