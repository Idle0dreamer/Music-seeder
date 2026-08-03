#include "Internal.hpp"

#include <string_view>

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

profile::Domain authorities(
    const Key& key,
    std::string_view kind) {
    profile::Domain result;
    const auto prefix = std::string(kind) + ".";
    for (const auto& [name, identity] : key.authorities) {
        if (name.starts_with(prefix)) {
            result.insert(identity);
        }
    }
    return result;
}

} // namespace

std::expected<std::shared_ptr<profile::Set>, std::string>
profile(const Key& key) {
    const auto& source = key.source;
    profile::Set base("maqam." + key.package + ".base");
    profile::Domain branchJins;
    profile::Domain branchPaths;
    profile::Domain routes;
    auto placeRoles = authorities(key, "role");
    auto centers = authorities(key, "center");
    auto jins = authorities(key, "jins");
    auto regions = authorities(key, "region");
    auto gestures = authorities(key, "gesture");
    auto phrases = authorities(key, "phrase");
    auto cells = authorities(key, "cell");
    auto formulas = authorities(key, "formula");
    auto motifs = authorities(key, "motif");
    auto cadences = authorities(key, "cadence");
    auto baggage = authorities(key, "baggage");
    centers.insert(key.centerRoot);
    jins.insert(key.jinsRoot);
    regions.insert(key.regionRoot);
    regions.insert(key.regionUpper);
    gestures.insert(key.gestureEstablish);
    gestures.insert(key.gestureAscent);
    gestures.insert(key.gestureDescent);
    gestures.insert(key.gestureResolution);
    phrases.insert(key.phraseQuestion);
    phrases.insert(key.phraseResponse);
    cells.insert(key.cellEstablish);
    cells.insert(key.cellDevelop);
    cells.insert(key.cellClimax);
    cells.insert(key.cellReturn);
    formulas.insert(key.formulaEstablish);
    formulas.insert(key.formulaDevelop);
    formulas.insert(key.formulaDevelopVariation);
    formulas.insert(key.formulaClimax);
    formulas.insert(key.formulaReturn);
    cadences.insert(key.cadenceLocal);
    cadences.insert(key.cadenceReturn);
    baggage.insert(key.baggageExtension);
    placeRoles.insert(key.roleTonic);
    placeRoles.insert(key.roleGhammaz);
    placeRoles.insert(key.roleUpper);
    placeRoles.insert(key.roleExtension);
    for (const auto& branch : key.branches) {
        branchJins.insert(branch.jins);
        branchPaths.insert(branch.path);
        routes.insert(branch.route);
        placeRoles.insert(branch.descent);
    }
    const std::vector<profile::Patch> rules{
        define("allow.anchor", std::move(centers), source),
        define("allow.enter", [&] {
            profile::Domain result = jins;
            result.insert(branchJins.begin(), branchJins.end());
            return result;
        }(), source),
        define("allow.emphasize", placeRoles, source),
        define("allow.dwell", placeRoles, source),
        define("allow.emit", cells, source),
        define("allow.variation", formulas, source),
        define("allow.motif", std::move(motifs), source),
        define("allow.cadence", cadences, source),
        define("allow.tonicize", branchJins, source),
        define("allow.modulate", branchPaths, source),
        define("allow.place", std::move(placeRoles), source),
        define("allow.register", std::move(regions), source),
        define("allow.baggage", std::move(baggage), source),
        define("allow.gesture", std::move(gestures), source),
        define("allow.phrase.function", std::move(phrases), source),
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
