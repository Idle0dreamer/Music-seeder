#include "Internal.hpp"

namespace mq::kernel::fixture::detail {
namespace ks = mq::kernel::sayr;
namespace {

ks::Need need(
    std::string name,
    ks::Requirement requirement) {
    return {
        Identity{"fixture.sayr.need", std::move(name), "1"},
        std::move(requirement),
    };
}

} // namespace

std::expected<ks::Plan, std::string> sayr(
    const Set& fixture) {
    const auto& obligation = fixture.sayr.obligation;
    const auto closed = phrase::Boundary::Closed;
    return ks::Plan::make(
        Identity{"fixture.sayr", "neutral", "1"},
        {
            {
                obligation.establish,
                {
                    need("establish.jins", ks::need::Jins{
                        fixture.jins.root,
                    }),
                    need("establish.role", ks::need::Role{
                        fixture.role.root,
                        1,
                    }),
                    need("establish.phrase", ks::need::Phrase{
                        fixture.phrase.function,
                        closed,
                        fixture.cadence,
                        1,
                    }),
                },
                {},
            },
            {
                obligation.settle,
                {
                    need("settle.center", ks::need::Center{
                        fixture.center.root,
                    }),
                    need("settle.jins", ks::need::Jins{
                        fixture.jins.root,
                    }),
                },
                {obligation.establish},
            },
            {
                obligation.expand,
                {
                    need("expand.gesture", ks::need::Gesture{
                        fixture.gesture.ascent,
                        1,
                    }),
                    need("expand.region", ks::need::Region{
                        fixture.region.upper,
                        2,
                    }),
                },
                {obligation.establish},
            },
            {
                obligation.climax,
                {
                    need("climax.role", ks::need::Role{
                        fixture.role.baggage,
                        1,
                    }),
                    need("climax.evidence", ks::need::Evidence{
                        evidence::Kind::Baggage,
                        Rational(1),
                    }),
                },
                {obligation.expand},
            },
            {
                obligation.travel,
                {
                    need("travel.jins", ks::need::Jins{
                        fixture.jins.branch,
                    }),
                    need("travel.center", ks::need::Center{
                        fixture.center.branch,
                    }),
                    need("travel.path", ks::need::Path{
                        fixture.path.direct,
                    }),
                },
                {obligation.climax},
            },
            {
                obligation.restore,
                {
                    need("restore.jins", ks::need::Jins{
                        fixture.jins.root,
                    }),
                    need("restore.center", ks::need::Center{
                        fixture.center.root,
                    }),
                    need("restore.gesture", ks::need::Gesture{
                        fixture.gesture.descent,
                        1,
                    }),
                    need("restore.role", ks::need::Role{
                        fixture.role.root,
                        2,
                    }),
                    need("restore.phrase", ks::need::Phrase{
                        fixture.phrase.function,
                        closed,
                        fixture.cadence,
                        2,
                    }),
                },
                {obligation.travel},
            },
        },
        {
            {
                fixture.sayr.route.stay,
                {obligation.settle},
            },
            {
                fixture.sayr.route.journey,
                {obligation.restore},
            },
        });
}

} // namespace mq::kernel::fixture::detail
