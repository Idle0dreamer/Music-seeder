#include "Internal.hpp"

namespace mq::kernel::maqam::family::detail {
namespace {

gesture::Definition one(
    const Identity& family,
    const Identity& role,
    const Identity& region,
    motion::Direction direction) {
    return {family, {{{role}, {region}, {direction}, std::nullopt}}};
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
    std::set<Identity> roles{
        key.roleTonic,
        key.roleGhammaz,
        key.roleUpper,
        key.roleExtension,
    };
    roles.insert(key.rootRoles.begin(), key.rootRoles.end());
    return {
        key.jinsRoot,
        key.roleTonic,
        std::move(roles),
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
        {key.source, "family:" + key.family, "scope:canonical-arabic-repertoire-v1"},
    };
}

jins::Descriptor upper(const Key& key, const BranchKey& branch) {
    const gesture::Definition descent = one(
        key.gestureDescent,
        branch.descent,
        key.regionUpper,
        branch.direction);
    return {
        branch.jins,
        branch.tonic,
        {branch.tonic, branch.ghammaz},
        {branch.ghammaz},
        {key.regionUpper},
        {key.regionUpper},
        {branch.tonic},
        {},
        {key.gestureDescent},
        {key.gestureDescent},
        {key.gestureDescent},
        {},
        {{descent.identity, descent}},
        {branch.source, "family:" + key.family, "scope:canonical-arabic-repertoire-v1"},
    };
}

} // namespace

std::expected<jins::Catalog, std::string> ajnas(const Key& key) {
    jins::Catalog result;
    auto add = result.add(root(key));
    if (!add) {
        return std::unexpected(add.error());
    }
    for (const auto& branch : key.branches) {
        add = result.add(upper(key, branch));
        if (!add) {
            return std::unexpected(add.error());
        }
    }
    return result;
}

} // namespace mq::kernel::maqam::family::detail
