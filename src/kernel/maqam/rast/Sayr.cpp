#include "Internal.hpp"

namespace mq::kernel::maqam::rast::detail {
namespace {

namespace ks = mq::kernel::sayr;

ks::Need need(std::string name, ks::Requirement requirement) {
    return {id("need." + std::move(name)), std::move(requirement)};
}

ks::Obligation travel(
    const Key& key,
    const Identity& obligation,
    const Identity& jins,
    const Identity& path,
    const std::string& name) {
    return {
        obligation,
        {
            need(name + ".jins", ks::need::Jins{jins}),
            need(name + ".center", ks::need::Center{key.centerUpper}),
            need(name + ".path", ks::need::Path{path}),
        },
        {key.obligationClimax},
    };
}

ks::Obligation restore(
    const Key& key,
    const Identity& obligation,
    const Identity& travelObligation,
    const std::string& name) {
    const auto closed = phrase::Boundary::Closed;
    return {
        obligation,
        {
            need(name + ".jins", ks::need::Jins{key.jinsRoot}),
            need(name + ".center", ks::need::Center{key.centerRoot}),
            need(name + ".resolution", ks::need::Gesture{
                key.gestureResolution,
                1,
            }),
            need(name + ".tonic", ks::need::Role{key.roleTonic, 2}),
            need(name + ".question", ks::need::Phrase{
                key.phraseQuestion,
                closed,
                key.cadenceLocal,
                1,
            }),
            need(name + ".response", ks::need::Phrase{
                key.phraseResponse,
                closed,
                key.cadenceReturn,
                1,
            }),
        },
        {travelObligation},
    };
}

} // namespace

std::expected<sayr::Plan, std::string> sayr(const Key& key) {
    namespace ks = mq::kernel::sayr;
    const auto closed = phrase::Boundary::Closed;
    auto result = ks::Plan::make(
        id("sayr.canonical"),
        {
            {
                key.obligationEstablish,
                {
                    need("establish.jins", ks::need::Jins{key.jinsRoot}),
                    need("establish.tonic", ks::need::Role{key.roleTonic, 1}),
                    need("establish.phrase", ks::need::Phrase{
                        key.phraseQuestion,
                        closed,
                        key.cadenceLocal,
                        1,
                    }),
                },
                {},
            },
            {
                key.obligationSettle,
                {
                    need("settle.center", ks::need::Center{key.centerRoot}),
                    need("settle.jins", ks::need::Jins{key.jinsRoot}),
                },
                {key.obligationEstablish},
            },
            {
                key.obligationExpand,
                {
                    need("expand.gesture", ks::need::Gesture{
                        key.gestureAscent,
                        1,
                    }),
                    need("expand.region", ks::need::Region{
                        key.regionUpper,
                        2,
                    }),
                },
                {key.obligationEstablish},
            },
            {
                key.obligationClimax,
                {
                    need("climax.extension", ks::need::Role{
                        key.roleExtension,
                        1,
                    }),
                    need("climax.baggage", ks::need::Evidence{
                        evidence::Kind::Baggage,
                        Rational(1),
                    }),
                },
                {key.obligationExpand},
            },
            travel(
                key,
                key.obligationTravelUpperRast,
                key.jinsUpperRast,
                key.pathUpperRast,
                "travel.upper-rast"),
            travel(
                key,
                key.obligationTravelNahawand,
                key.jinsUpperNahawand,
                key.pathNahawand,
                "travel.nahawand"),
            restore(
                key,
                key.obligationRestoreUpperRast,
                key.obligationTravelUpperRast,
                "restore.upper-rast"),
            restore(
                key,
                key.obligationRestoreNahawand,
                key.obligationTravelNahawand,
                "restore.nahawand"),
        },
        {
            {key.routeStay, {key.obligationSettle}},
            {key.routeUpperRast, {key.obligationRestoreUpperRast}},
            {key.routeNahawand, {key.obligationRestoreNahawand}},
        });
    if (!result) {
        return std::unexpected(result.error());
    }
    return result;
}

} // namespace mq::kernel::maqam::rast::detail
