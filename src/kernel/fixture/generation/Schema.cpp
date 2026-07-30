#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

pitch::field::Schema schema(
    const fixture::Set& fixture,
    const Key& key) {
    namespace pf = pitch::field;

    const auto tier = id("tier");
    const auto anchor = id("rule.anchor");
    const auto stay = id("rule.stay");
    const auto root = id("rule.gesture.root");
    const auto ghammaz = id("rule.gesture.ghammaz");
    const auto extension = id("rule.gesture.extension");
    const auto returnGhammaz = id("rule.return.ghammaz");
    const auto returnRoot = id("rule.return.root");
    return {
        {
            fixture.role.root,
            fixture.role.ghammaz,
            fixture.role.baggage,
        },
        {tier},
        {
            {
                {},
                pitch::Equation{
                    anchor,
                    {{fixture.role.root, Rational(1)}},
                    {},
                    "neutral generated root anchor",
                },
            },
            {
                {{
                    {key.role, fixture.role.root},
                    {key.function, fixture.phrase.function},
                    {key.cadence, fixture.cadence},
                    {key.boundary, key.closed},
                }},
                pf::Aim{
                    tier,
                    {
                        stay,
                        {{fixture.role.root, Rational(1)}},
                        {},
                        Rational(1),
                        "neutral closed root target",
                    },
                },
            },
            {
                {{
                    {key.role, fixture.role.root},
                    {key.motion, key.start},
                    {key.region, fixture.region.root},
                    {key.gesture, fixture.gesture.ascent},
                }},
                pf::Aim{
                    tier,
                    {
                        root,
                        {{fixture.role.root, Rational(1)}},
                        {},
                        Rational(1),
                        "neutral gesture root target",
                    },
                },
            },
            {
                {{
                    {key.role, fixture.role.ghammaz},
                    {key.motion, key.rise},
                    {key.region, fixture.region.upper},
                    {key.gesture, fixture.gesture.ascent},
                }},
                pf::Aim{
                    tier,
                    {
                        ghammaz,
                        {
                            {fixture.role.ghammaz, Rational(1)},
                            {fixture.role.root, Rational(-1)},
                        },
                        pitch::Expression::ratio(4, 3),
                        Rational(1),
                        "neutral gesture ghammaz relation",
                    },
                },
            },
            {
                {{
                    {key.role, fixture.role.baggage},
                    {key.motion, key.rise},
                    {key.region, fixture.region.upper},
                    {key.baggage, fixture.baggage},
                    {key.gesture, fixture.gesture.ascent},
                    {key.cadence, fixture.cadence},
                    {key.boundary, key.closed},
                }},
                pf::Aim{
                    tier,
                    {
                        extension,
                        {
                            {fixture.role.baggage, Rational(1)},
                            {fixture.role.root, Rational(-1)},
                        },
                        pitch::Expression::ratio(3, 2),
                        Rational(1),
                        "neutral gesture baggage relation",
                    },
                },
            },
            {
                {{
                    {key.role, fixture.role.ghammaz},
                    {key.motion, key.fall},
                    {key.region, fixture.region.upper},
                    {key.gesture, fixture.gesture.descent},
                }},
                pf::Aim{
                    tier,
                    {
                        returnGhammaz,
                        {
                            {fixture.role.ghammaz, Rational(1)},
                            {fixture.role.root, Rational(-1)},
                        },
                        pitch::Expression::ratio(4, 3),
                        Rational(1),
                        "neutral return ghammaz relation",
                    },
                },
            },
            {
                {{
                    {key.role, fixture.role.root},
                    {key.motion, key.fall},
                    {key.region, fixture.region.root},
                    {key.gesture, fixture.gesture.descent},
                    {key.cadence, fixture.cadence},
                    {key.boundary, key.closed},
                }},
                pf::Aim{
                    tier,
                    {
                        returnRoot,
                        {{fixture.role.root, Rational(1)}},
                        {},
                        Rational(1),
                        "neutral return root target",
                    },
                },
            },
        },
        {{
            id("need"),
            {
                stay,
                root,
                ghammaz,
                extension,
                returnGhammaz,
                returnRoot,
            },
        }},
    };
}

} // namespace mq::kernel::fixture::generation::detail
