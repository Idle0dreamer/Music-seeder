#include "mq/kernel/Fixture.hpp"

namespace mq::kernel {
namespace {

Rule domainRule(Domain values, std::string provenance) {
    return Rule{std::move(values), {std::move(provenance)}};
}

Rule parameterRule(Rational value, std::string provenance) {
    return Rule{value, {std::move(provenance)}};
}

Patch defineDomain(
    std::string key,
    Domain values,
    const std::string& source) {
    return Patch{
        PatchAction::Define,
        std::move(key),
        domainRule(std::move(values), source),
    };
}

Patch defineParameter(
    std::string key,
    Rational value,
    const std::string& source) {
    return Patch{
        PatchAction::Define,
        std::move(key),
        parameterRule(value, source),
    };
}

} // namespace

std::expected<Fixture, std::string> makeFixture() {
    const Identity rootCenter{"fixture.center", "root", "1"};
    const Identity branchCenter{"fixture.center", "branch", "1"};
    const Identity rootJins{"fixture.jins", "root", "1"};
    const Identity branchJins{"fixture.jins", "branch", "1"};
    const Identity rootRole{"fixture.role", "root", "1"};
    const Identity ghammazRole{"fixture.role", "ghammaz", "1"};
    const Identity cell{"fixture.cell", "establish", "1"};
    const Identity cadence{"fixture.cadence", "local", "1"};
    const Identity path{"fixture.path", "root-to-branch", "1"};

    const std::string baseSource = "fixture:shared-semantics";
    const std::vector<Patch> basePatches{
        defineDomain("allow.anchor", {rootCenter}, baseSource),
        defineDomain("allow.enter", {rootJins, branchJins}, baseSource),
        defineDomain(
            "allow.emphasize",
            {rootRole, ghammazRole},
            baseSource),
        defineDomain("allow.dwell", {rootRole, ghammazRole}, baseSource),
        defineDomain("allow.emit", {cell}, baseSource),
        defineDomain("allow.cadence", {cadence}, baseSource),
        defineDomain("allow.tonicize", {branchJins}, baseSource),
        defineDomain("allow.modulate", {path}, baseSource),
        defineParameter(
            "threshold.internal.emphasis",
            Rational(2),
            baseSource),
        defineParameter(
            "threshold.internal.dwell",
            Rational(2),
            baseSource),
        defineParameter(
            "threshold.internal.cell",
            Rational(1),
            baseSource),
        defineParameter(
            "threshold.internal.cadence",
            Rational(1),
            baseSource),
    };

    auto shared = reconstruct("fixture.shared", {}, basePatches);
    if (!shared) {
        return std::unexpected(shared.error());
    }

    const std::vector<Patch> regionalAPatches{
        Patch{
            PatchAction::Parameterize,
            "threshold.internal.emphasis",
            parameterRule(Rational(3), "fixture:regional-a"),
        },
    };
    auto regionalA =
        reconstruct("fixture.regional-a", {*shared}, regionalAPatches);
    if (!regionalA) {
        return std::unexpected(regionalA.error());
    }

    const std::vector<Patch> regionalBPatches{
        Patch{
            PatchAction::Forbid,
            "allow.modulate",
            Rule{false, {"fixture:regional-b"}},
        },
    };
    auto regionalB =
        reconstruct("fixture.regional-b", {*shared}, regionalBPatches);
    if (!regionalB) {
        return std::unexpected(regionalB.error());
    }

    return Fixture{
        rootCenter,
        branchCenter,
        rootJins,
        branchJins,
        rootRole,
        ghammazRole,
        cell,
        cadence,
        path,
        std::move(*shared),
        std::move(*regionalA),
        std::move(*regionalB),
    };
}

} // namespace mq::kernel

