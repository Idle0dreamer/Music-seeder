#include "Internal.hpp"

namespace mq::kernel::maqam::bayati::detail {
namespace {

profile::Patch define(
    std::string key,
    profile::Domain value,
    const std::string& source) {
    return {
        profile::Patch::Action::Define,
        std::move(key),
        profile::Rule{std::move(value), {source}},
    };
}

profile::Patch parameter(
    std::string key,
    Rational value,
    const std::string& source) {
    return {
        profile::Patch::Action::Define,
        std::move(key),
        profile::Rule{value, {source}},
    };
}

} // namespace

std::expected<std::shared_ptr<profile::Set>, std::string>
profile(const Key& key) {
    // The evidence envelope is deliberately scoped: Egypt and the Levant,
    // literature-derived, provisional, and not a regional authenticity claim.
    const std::string source =
        "AbuShumays2013:pp.3-6,22-24;MaqamWorld:maqam-bayati";
    profile::Set base("maqam.bayati.base");
    const std::vector<profile::Patch> rules{
        define("allow.anchor", {key.centerRoot}, source),
        define("allow.enter", {
            key.jinsRoot,
            key.jinsNahawand,
            key.jinsRast,
            key.jinsHijaz,
        }, source),
        define("allow.emphasize", {key.roleGhammaz}, source),
        define("allow.dwell", {key.roleGhammaz}, source),
        define("allow.emit", {
            key.cellEstablish,
            key.cellDevelop,
            key.cellClimax,
            key.cellReturn,
        }, source),
        define("allow.cadence", {key.cadenceLocal, key.cadenceReturn}, source),
        define("allow.tonicize", {
            key.jinsNahawand,
            key.jinsRast,
            key.jinsHijaz,
        }, source),
        define("allow.modulate", {
            key.pathNahawand,
            key.pathRast,
            key.pathHijaz,
        }, source),
        define("allow.place", {
            key.roleTonic,
            key.roleSikah,
            key.roleJaharkah,
            key.roleGhammaz,
            key.roleHusayni,
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
            key.phraseEstablish,
            key.phraseReturn,
        }, source),
        parameter("threshold.internal.emphasis", Rational(2), source),
        parameter("threshold.internal.dwell", Rational(2), source),
        parameter("threshold.internal.cell", Rational(1), source),
        parameter("threshold.internal.baggage", Rational(1), source),
        parameter("threshold.internal.cadence", Rational(1), source),
        parameter("band.generation", Rational(8), source),
    };
    auto result = profile::reconstruct(
        "maqam.bayati.provisional.egypt-levant.v1",
        {base},
        rules);
    if (!result) {
        return std::unexpected(result.error());
    }
    return std::make_shared<profile::Set>(std::move(*result));
}

} // namespace mq::kernel::maqam::bayati::detail
