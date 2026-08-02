#include "Internal.hpp"

namespace mq::kernel::maqam::family::detail {
namespace {

profile::Patch define(
    std::string name,
    profile::Domain domain,
    const std::string& source) {
    return {
        profile::Patch::Action::Define,
        std::move(name),
        profile::Rule{std::move(domain), {source}},
    };
}

profile::Patch parameter(
    std::string name,
    Rational value,
    const std::string& source) {
    return {
        profile::Patch::Action::Define,
        std::move(name),
        profile::Rule{value, {source}},
    };
}

} // namespace

std::expected<std::shared_ptr<profile::Set>, std::string>
profile(const Key& key) {
    const auto& source = key.source;
    profile::Set base("maqam." + key.package + ".base");
    profile::Domain branchJins;
    profile::Domain branchPaths;
    profile::Domain routes;
    for (const auto& branch : key.branches) {
        branchJins.insert(branch.jins);
        branchPaths.insert(branch.path);
        routes.insert(branch.route);
    }
    const std::vector<profile::Patch> rules{
        define("allow.anchor", {key.centerRoot}, source),
        define("allow.enter", [&] {
            profile::Domain result{key.jinsRoot};
            result.insert(branchJins.begin(), branchJins.end());
            return result;
        }(), source),
        define("allow.emphasize", {key.roleGhammaz}, source),
        define("allow.dwell", {key.roleGhammaz}, source),
        define("allow.emit", {
            key.cellEstablish,
            key.cellDevelop,
            key.cellClimax,
            key.cellReturn,
        }, source),
        define("allow.variation", {
            key.formulaEstablish,
            key.formulaDevelop,
            key.formulaDevelopVariation,
            key.formulaClimax,
            key.formulaReturn,
        }, source),
        define("allow.cadence", {key.cadenceLocal, key.cadenceReturn}, source),
        define("allow.tonicize", branchJins, source),
        define("allow.modulate", branchPaths, source),
        define("allow.place", {
            key.roleTonic,
            key.roleGhammaz,
            key.roleUpper,
            key.roleExtension,
        }, source),
        define("allow.register", {key.regionRoot, key.regionUpper}, source),
        define("allow.baggage", {key.baggageExtension}, source),
        define("allow.gesture", {
            key.gestureEstablish,
            key.gestureAscent,
            key.gestureDescent,
            key.gestureResolution,
        }, source),
        define("allow.phrase.function", {
            key.phraseQuestion,
            key.phraseResponse,
        }, source),
        parameter("threshold.internal.emphasis", Rational(2), source),
        parameter("threshold.internal.dwell", Rational(2), source),
        parameter("threshold.internal.cell", Rational(1), source),
        parameter("threshold.internal.baggage", Rational(1), source),
        parameter("threshold.internal.cadence", Rational(1), source),
        parameter("band.generation", Rational(8), source),
    };
    auto result = profile::reconstruct(
        "maqam." + key.package + "." + key.family + ".canonical.v1",
        {base},
        rules);
    if (!result) {
        return std::unexpected(result.error());
    }
    return std::make_shared<profile::Set>(std::move(*result));
}

} // namespace mq::kernel::maqam::family::detail
