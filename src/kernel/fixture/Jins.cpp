#include "Internal.hpp"

namespace mq::kernel::fixture::detail {
namespace {

jins::Descriptor descriptor(
    const Set& fixture,
    const Identity& identity) {
    const gesture::Definition ascent{
        fixture.gesture.ascent,
        {
            {
                {fixture.role.root},
                {fixture.region.root},
                {motion::Direction::Start},
                std::nullopt,
            },
            {
                {fixture.role.ghammaz},
                {fixture.region.upper},
                {motion::Direction::Rise},
                std::nullopt,
            },
            {
                {fixture.role.baggage},
                {fixture.region.upper},
                {motion::Direction::Rise},
                fixture.baggage,
            },
        },
    };
    const gesture::Definition descent{
        fixture.gesture.descent,
        {
            {
                {fixture.role.ghammaz},
                {fixture.region.upper},
                {motion::Direction::Fall},
                std::nullopt,
            },
            {
                {fixture.role.root},
                {fixture.region.root},
                {motion::Direction::Fall},
                std::nullopt,
            },
        },
    };
    const jins::Baggage baggage{
        fixture.baggage,
        fixture.role.baggage,
        {fixture.region.upper},
        {motion::Direction::Rise, motion::Direction::Fall},
        {fixture.gesture.ascent},
    };
    return {
        identity,
        fixture.role.root,
        {
            fixture.role.root,
            fixture.role.ghammaz,
            fixture.role.baggage,
        },
        {fixture.role.ghammaz},
        {fixture.region.root, fixture.region.upper},
        {fixture.region.root, fixture.region.upper},
        {fixture.role.root, fixture.role.ghammaz},
        {{baggage.identity, baggage}},
        {
            {ascent.identity, ascent},
            {descent.identity, descent},
        },
    };
}

} // namespace

std::expected<jins::Catalog, std::string> catalog(
    const Set& fixture) {
    jins::Catalog result;
    auto root = result.add(descriptor(fixture, fixture.jins.root));
    if (!root) {
        return std::unexpected(root.error());
    }
    auto branch = result.add(descriptor(fixture, fixture.jins.branch));
    if (!branch) {
        return std::unexpected(branch.error());
    }
    return result;
}

} // namespace mq::kernel::fixture::detail
