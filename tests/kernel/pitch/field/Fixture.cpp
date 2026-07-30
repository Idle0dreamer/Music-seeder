#include "Fixture.hpp"

namespace test::field {
namespace {

mq::kernel::Identity id(
    std::string domain,
    std::string name) {
    return {std::move(domain), std::move(name), "1"};
}

} // namespace

Fixture fixture() {
    using namespace mq::kernel;
    namespace pf = pitch::field;

    Fixture value{
        id("test.field.variable", "root"),
        id("test.field.variable", "upper"),
        id("test.field.context.key", "direction"),
        id("test.field.context.value", "up"),
        id("test.field.context.value", "down"),
        id("test.field.tier", "structural"),
        id("test.field.tier", "surface"),
        id("test.field.rule", "anchor"),
        id("test.field.rule", "rising"),
        id("test.field.rule", "falling"),
        id("test.field.rule", "surface"),
        {},
    };
    const auto inactive = id("test.field.tier", "inactive");
    const auto coverage = id("test.field.need", "direction");

    value.schema.variables = {value.root, value.upper};
    value.schema.tiers = {value.strong, inactive, value.weak};
    value.schema.rules = {
        {
            {},
            pitch::Equation{
                value.anchor,
                {{value.root, Rational(1)}},
                {},
                "neutral root anchor",
            },
        },
        {
            {{{value.direction, value.up}}},
            pf::Aim{
                value.strong,
                {
                    value.rising,
                    {{value.upper, Rational(1)},
                     {value.root, Rational(-1)}},
                    pitch::Expression::ratio(4, 3),
                    Rational(1),
                    "neutral rising interval",
                },
            },
        },
        {
            {{{value.direction, value.down}}},
            pf::Aim{
                value.strong,
                {
                    value.falling,
                    {{value.upper, Rational(1)},
                     {value.root, Rational(-1)}},
                    pitch::Expression::cents(Rational(500)),
                    Rational(1),
                    "neutral falling interval",
                },
            },
        },
        {
            {},
            pf::Aim{
                value.weak,
                {
                    value.surface,
                    {{value.upper, Rational(1)}},
                    {},
                    Rational(1),
                    "neutral surface tendency",
                },
            },
        },
    };
    value.schema.needs = {{
        coverage,
        {value.rising, value.falling},
    }};
    return value;
}

} // namespace test::field
