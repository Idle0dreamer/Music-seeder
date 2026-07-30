#include "../../Test.hpp"
#include "Fixture.hpp"

#include "mq/kernel/pitch/field/assemble/Run.hpp"

#include <algorithm>

void test::field::assembly() {
    using namespace mq::kernel;
    namespace pa = pitch::field::assemble;

    auto value = fixture();
    const Identity extraKey{
        "test.field.context.key", "register", "1"};
    const Identity extraValue{
        "test.field.context.value", "middle", "1"};
    pitch::field::Context context{{
        {value.direction, value.up},
        {extraKey, extraValue},
    }};
    const auto first = pa::run(value.schema, context);
    require(
        first &&
            first->rules.size() == 3 &&
            first->tiers.size() == 2 &&
            first->tiers[0].identity == value.strong &&
            first->tiers[1].identity == value.weak &&
            std::ranges::find(first->rules, value.rising) !=
                first->rules.end() &&
            std::ranges::find(first->rules, value.falling) ==
                first->rules.end(),
        "context did not select and order the exact active rule set");

    const auto minimal = pa::run(
        value.schema,
        pitch::field::Context{{{value.direction, value.up}}});
    require(
        minimal &&
            minimal->rules == first->rules &&
            minimal->tiers.size() == first->tiers.size(),
        "unrelated context fact changed field assembly");

    std::ranges::reverse(value.schema.rules);
    std::ranges::reverse(context.facts);
    const auto reordered = pa::run(value.schema, context);
    require(
        reordered &&
            reordered->variables == first->variables &&
            reordered->rules == first->rules &&
            reordered->tiers[0].identity == first->tiers[0].identity &&
            reordered->tiers[1].identity == first->tiers[1].identity,
        "field or context storage order changed assembly");

    const pitch::field::Context uncovered{{{extraKey, extraValue}}};
    const auto missing = pa::run(value.schema, uncovered);
    require(
        !missing &&
            missing.error().code == pa::Error::Code::Coverage,
        "missing contextual coverage used an implicit fallback");

    auto duplicate = context;
    duplicate.facts.push_back({value.direction, value.down});
    const auto repeated = pa::run(value.schema, duplicate);
    require(
        !repeated &&
            repeated.error().code == pa::Error::Code::Input,
        "duplicate context key was accepted");

    const auto limited = pa::run(
        value.schema,
        context,
        pa::Limits{64, 2, 512});
    require(
        !limited &&
            limited.error().code == pa::Error::Code::Count,
        "field rule budget exhaustion did not fail");
}
