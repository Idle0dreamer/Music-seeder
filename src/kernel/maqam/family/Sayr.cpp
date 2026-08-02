#include "Internal.hpp"

namespace mq::kernel::maqam::family::detail {
namespace {

namespace ks = mq::kernel::sayr;

ks::Need need(std::string name, ks::Requirement value) {
    return {Identity{"maqam.need", std::move(name), "1"}, std::move(value)};
}

ks::Obligation travel(
    const Key& key,
    const BranchKey& branch) {
    return {
        branch.travel,
        {
            need(branch.travel.name + ".jins", ks::need::Jins{branch.jins}),
            need(branch.travel.name + ".center", ks::need::Center{key.centerUpper}),
            need(branch.travel.name + ".path", ks::need::Path{branch.path}),
        },
        {id(key, "obligation.climax")},
    };
}

ks::Obligation restore(
    const Key& key,
    const BranchKey& branch) {
    const auto closed = phrase::Boundary::Closed;
    return {
        branch.restore,
        {
            need(branch.restore.name + ".jins", ks::need::Jins{key.jinsRoot}),
            need(branch.restore.name + ".center", ks::need::Center{key.centerRoot}),
            need(branch.restore.name + ".resolution", ks::need::Gesture{
                key.gestureResolution,
                1,
            }),
            need(branch.restore.name + ".tonic", ks::need::Role{key.roleTonic, 2}),
            need(branch.restore.name + ".question", ks::need::Phrase{
                key.phraseQuestion,
                closed,
                key.cadenceLocal,
                1,
            }),
            need(branch.restore.name + ".response", ks::need::Phrase{
                key.phraseResponse,
                closed,
                key.cadenceReturn,
                1,
            }),
        },
        {branch.travel},
    };
}

} // namespace

std::expected<sayr::Plan, std::string> sayr(const Key& key) {
    namespace ks = mq::kernel::sayr;
    const auto closed = phrase::Boundary::Closed;
    std::vector<ks::Obligation> obligations{
        {
            id(key, "obligation.establish"),
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
            id(key, "obligation.settle"),
            {
                need("settle.center", ks::need::Center{key.centerRoot}),
                need("settle.jins", ks::need::Jins{key.jinsRoot}),
            },
            {id(key, "obligation.establish")},
        },
        {
            id(key, "obligation.expand"),
            {
                need("expand.gesture", ks::need::Gesture{key.gestureAscent, 1}),
                need("expand.region", ks::need::Region{key.regionUpper, 2}),
            },
            {id(key, "obligation.establish")},
        },
        {
            id(key, "obligation.climax"),
            {
                need("climax.extension", ks::need::Role{key.roleExtension, 1}),
                need("climax.baggage", ks::need::Evidence{
                    evidence::Kind::Baggage,
                    Rational(1),
                }),
            },
            {id(key, "obligation.expand")},
        },
    };
    std::vector<ks::Route> routes{
        {
            id(key, "route.stay"),
            {id(key, "obligation.settle")},
        },
    };
    for (const auto& branch : key.branches) {
        obligations.push_back(travel(key, branch));
        obligations.push_back(restore(key, branch));
        routes.push_back({branch.route, {branch.restore}});
    }
    auto result = ks::Plan::make(
        id(key, "sayr.canonical"),
        std::move(obligations),
        std::move(routes));
    if (!result) {
        return std::unexpected(result.error());
    }
    return result;
}

} // namespace mq::kernel::maqam::family::detail
