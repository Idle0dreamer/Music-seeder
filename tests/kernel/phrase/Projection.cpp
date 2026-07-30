#include "../Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/pitch/request/Run.hpp"

#include <algorithm>

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.phrase.project", std::move(name), "1"};
}

std::optional<mq::kernel::Identity> value(
    const mq::kernel::pitch::field::Context& context,
    const mq::kernel::Identity& key) {
    const auto found = std::ranges::find(
        context.facts,
        key,
        &mq::kernel::pitch::field::Fact::key);
    if (found == context.facts.end()) {
        return std::nullopt;
    }
    return found->value;
}

} // namespace

void test::phrase::projection() {
    using namespace mq::kernel;
    namespace pf = pitch::field;
    namespace pp = pf::project;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const eval::Evaluator evaluator(fixture.profile.shared);
    const auto phraseId = id("phrase");
    const auto eventId = id("event");
    const std::vector<operation::Any> program{
        operation::Begin{
            phraseId,
            mq::kernel::phrase::Function{fixture.phrase.function},
        },
        operation::Place{
            eventId,
            fixture.role.root,
            motion::Direction::Start,
        },
        operation::Cadence{
            fixture.cadence,
            Rational(1),
            Rational(3, 4),
        },
        operation::End{
            phraseId,
            mq::kernel::phrase::Boundary::Closed,
        },
    };
    const auto state = evaluator.run({}, program);
    require(state.has_value(), state ? "" : state.error().message);

    const auto functionKey = id("key.function");
    const auto cadenceKey = id("key.cadence");
    const auto boundaryKey = id("key.boundary");
    const auto roleKey = id("key.role");
    const auto open = id("boundary.open");
    const auto closed = id("boundary.closed");
    const pp::Plan projection{{
        pp::phrase::boundary::Read{boundaryKey, open, closed},
        pp::phrase::function::Read{functionKey},
        pp::role::Read{roleKey},
        pp::phrase::cadence::Read{cadenceKey},
    }};
    const auto context = pp::run(projection, *state);
    require(
        context &&
            value(*context, functionKey) == fixture.phrase.function &&
            value(*context, cadenceKey) == fixture.cadence &&
            value(*context, boundaryKey) == closed &&
            value(*context, roleKey) == fixture.role.root,
        "completed phrase state projected incorrect typed facts");

    const auto tier = id("tier");
    const auto rule = id("rule");
    const auto need = id("need");
    const auto target = pitch::Expression::ratio(9, 8);
    const pf::Schema schema{
        {fixture.role.root},
        {tier},
        {{
            {{
                {functionKey, fixture.phrase.function},
                {cadenceKey, fixture.cadence},
                {boundaryKey, closed},
                {roleKey, fixture.role.root},
            }},
            pf::Aim{
                tier,
                {
                    rule,
                    {{fixture.role.root, Rational(1)}},
                    target,
                    Rational(1),
                    "neutral phrase-conditioned target",
                },
            },
        }},
        {{need, {rule}}},
    };
    const auto request = pitch::request::run(
        *state,
        projection,
        schema);
    require(
        request &&
            request->plan.targets.size() == 1 &&
            request->plan.targets.front().center == target &&
            std::ranges::find(request->field.rules, rule) !=
                request->field.rules.end(),
        "phrase context did not select an exact pitch request");

    const pp::Plan missing{{
        pp::phrase::boundary::Read{boundaryKey, open, closed},
    }};
    const auto absent = pp::run(missing, {});
    require(
        !absent && absent.error().code == pp::Error::Code::Missing,
        "required phrase boundary was silently omitted");
}
