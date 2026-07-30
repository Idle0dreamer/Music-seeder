#include "../../Test.hpp"
#include "Fixture.hpp"

#include "mq/kernel/pitch/field/solve/Run.hpp"

#include <variant>

void test::field::solve() {
    using namespace mq::kernel;
    namespace ps = pitch::field::solve;

    auto value = fixture();
    const pitch::field::Context context{{
        {value.direction, value.up},
    }};
    const auto result = ps::run(value.schema, context);
    require(
        result &&
            result->solution.values.at(value.root) ==
                pitch::Expression{} &&
            result->solution.values.at(value.upper) ==
                pitch::Expression::ratio(4, 3) &&
            result->steps.size() == 2 &&
            result->steps[0].predictions.at(value.rising) ==
                pitch::Expression::ratio(4, 3) &&
            result->steps[1].predictions.at(value.surface) ==
                pitch::Expression::ratio(4, 3),
        "assembled field lost exact ratio or authority ordering");

    auto invalid = value.schema;
    const Identity unknown{"test.field.tier", "unknown", "1"};
    for (auto& rule : invalid.rules) {
        if (std::holds_alternative<pitch::field::Aim>(rule.effect)) {
            auto& aim = std::get<pitch::field::Aim>(rule.effect);
            if (aim.target.identity == value.rising) {
                aim.tier = unknown;
            }
        }
    }
    const auto rejected = ps::run(invalid, context);
    require(
        !rejected &&
            rejected.error().code == ps::Error::Code::Assembly &&
            rejected.error().assembly &&
            rejected.error().assembly->code ==
                pitch::field::assemble::Error::Code::Input,
        "field accepted a target without declared authority");

    auto conflict = value.schema;
    conflict.rules.push_back({
        {{{value.direction, value.up}}},
        pitch::Equation{
            {"test.field.rule", "conflict", "1"},
            {{value.root, Rational(1)}},
            pitch::Expression::cents(Rational(1)),
            "neutral deliberate contradiction",
        },
    });
    const auto contradicted = ps::run(conflict, context);
    require(
        !contradicted &&
            contradicted.error().code == ps::Error::Code::Hard,
        "context-selected hard contradiction was mislabeled or averaged");

    auto hard = value.schema;
    std::erase_if(
        hard.rules,
        [](const pitch::field::Rule& rule) {
            return !std::holds_alternative<pitch::Equation>(rule.effect);
        });
    hard.tiers.clear();
    hard.needs.clear();
    const auto hardOnly = ps::run(hard, {});
    require(
        hardOnly &&
            hardOnly->steps.empty() &&
            hardOnly->solution.values.at(value.root) ==
                pitch::Expression{},
        "hard-only assembled field did not produce an exact witness");
}
