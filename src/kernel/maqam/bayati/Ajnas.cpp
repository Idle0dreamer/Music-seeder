#include "Internal.hpp"

namespace mq::kernel::maqam::bayati::detail {
namespace {

gesture::Definition one(
    const Identity& family,
    const Identity& role,
    const Identity& region,
    motion::Direction direction) {
    return {
        family,
        {{{role}, {region}, {direction}, std::nullopt}},
    };
}

jins::Descriptor root(const Key& key) {
    const gesture::Definition establish = one(
        key.gestureEstablish,
        key.roleTonic,
        key.regionRoot,
        motion::Direction::Start);
    const gesture::Definition ascent{
        key.gestureAscent,
        {
            {{key.roleTonic}, {key.regionRoot}, {motion::Direction::Start}, std::nullopt},
            {{key.roleGhammaz}, {key.regionUpper}, {motion::Direction::Rise}, std::nullopt},
            {{key.roleExtension}, {key.regionUpper}, {motion::Direction::Rise}, key.baggageExtension},
        },
    };
    const gesture::Definition resolution = one(
        key.gestureResolution,
        key.roleTonic,
        key.regionRoot,
        motion::Direction::Fall);
    const jins::Baggage baggage{
        key.baggageExtension,
        key.roleExtension,
        {key.regionUpper},
        {motion::Direction::Rise},
        {key.gestureAscent},
    };
    return {
        key.jinsRoot,
        key.roleTonic,
        {
            key.roleTonic,
            key.roleSikah,
            key.roleJaharkah,
            key.roleGhammaz,
            key.roleExtension,
        },
        {key.roleGhammaz},
        {key.regionRoot, key.regionUpper},
        {key.regionRoot, key.regionUpper},
        {key.roleTonic, key.roleGhammaz},
        {key.gestureEstablish},
        {key.gestureResolution},
        {key.gestureResolution},
        {key.gestureAscent},
        {{baggage.identity, baggage}},
        {
            {establish.identity, establish},
            {ascent.identity, ascent},
            {resolution.identity, resolution},
        },
        {
            "MaqamWorld:maqam-bayati",
            "AbuShumays2013:pp.3-6",
            "status:provisional;scope:Egypt-Levant;not-authenticity-validated",
        },
    };
}

jins::Descriptor upper(
    const Key& key,
    const Identity& identity) {
    const gesture::Definition descent = one(
        key.gestureDescent,
        key.roleGhammaz,
        key.regionUpper,
        motion::Direction::Fall);
    return {
        identity,
        key.roleGhammaz,
        {key.roleGhammaz, key.roleHusayni},
        {key.roleHusayni},
        {key.regionUpper},
        {key.regionUpper},
        {key.roleGhammaz},
        {},
        {key.gestureDescent},
        {key.gestureDescent},
        {key.gestureDescent},
        {},
        {{descent.identity, descent}},
        {
            "MaqamWorld:maqam-bayati",
            "AbuShumays2013:pp.3-4,22-24",
            "status:provisional;upper-jins-descent-only",
        },
    };
}

} // namespace

std::expected<jins::Catalog, std::string> ajnas(const Key& key) {
    jins::Catalog result;
    for (const auto& descriptor : {
             root(key),
             upper(key, key.jinsNahawand),
             upper(key, key.jinsRast),
             upper(key, key.jinsHijaz),
         }) {
        auto added = result.add(descriptor);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    return result;
}

} // namespace mq::kernel::maqam::bayati::detail
