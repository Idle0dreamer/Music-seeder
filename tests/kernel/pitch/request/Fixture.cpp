#include "Fixture.hpp"

namespace test::request {
namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.request", std::move(name), "1"};
}

} // namespace

Case make(const mq::kernel::fixture::Set& fixture) {
    using namespace mq::kernel;
    namespace pf = pitch::field;
    namespace pp = pf::project;

    const auto roleKey = id("context.role");
    const auto motionKey = id("context.motion");
    const auto start = id("motion.start");
    const auto same = id("motion.same");
    const auto rise = id("motion.rise");
    const auto fall = id("motion.fall");
    const auto tier = id("tier.structural");
    const auto startRule = id("rule.start");
    const auto riseRule = id("rule.rise");
    const auto sameRule = id("rule.same");
    const auto fallRule = id("rule.fall");

    return {
        id("event.first"),
        id("event.second"),
        id("event.third"),
        id("event.fourth"),
        riseRule,
        {{
            pp::role::Read{roleKey},
            pp::motion::Read{
                motionKey,
                start,
                same,
                rise,
                fall,
            },
        }},
        {
            {fixture.role.root, fixture.role.ghammaz},
            {tier},
            {
                {
                    {},
                    pitch::Equation{
                        id("rule.anchor"),
                        {{fixture.role.root, Rational(1)}},
                        {},
                        "neutral request anchor",
                    },
                },
                {
                    {{
                        {roleKey, fixture.role.root},
                        {motionKey, start},
                    }},
                    pf::Aim{
                        tier,
                        {
                            startRule,
                            {{fixture.role.root, Rational(1)}},
                            {},
                            Rational(1),
                            "neutral start target",
                        },
                    },
                },
                {
                    {{
                        {roleKey, fixture.role.ghammaz},
                        {motionKey, rise},
                    }},
                    pf::Aim{
                        tier,
                        {
                            riseRule,
                            {
                                {fixture.role.ghammaz, Rational(1)},
                                {fixture.role.root, Rational(-1)},
                            },
                            pitch::Expression::ratio(4, 3),
                            Rational(1),
                            "neutral rising target",
                        },
                    },
                },
                {
                    {{
                        {roleKey, fixture.role.ghammaz},
                        {motionKey, same},
                    }},
                    pf::Aim{
                        tier,
                        {
                            sameRule,
                            {
                                {fixture.role.ghammaz, Rational(1)},
                                {fixture.role.root, Rational(-1)},
                            },
                            pitch::Expression::ratio(4, 3),
                            Rational(1),
                            "neutral repeated target",
                        },
                    },
                },
                {
                    {{
                        {roleKey, fixture.role.root},
                        {motionKey, fall},
                    }},
                    pf::Aim{
                        tier,
                        {
                            fallRule,
                            {{fixture.role.root, Rational(1)}},
                            {},
                            Rational(1),
                            "neutral falling target",
                        },
                    },
                },
            },
            {{
                id("need.event"),
                {startRule, riseRule, sameRule, fallRule},
            }},
        },
    };
}

} // namespace test::request
