#include "Internal.hpp"

namespace mq::kernel::fixture::generation {

std::expected<Set, std::string> make(
    const fixture::Set& fixture) {
    namespace pf = pitch::field;
    namespace pp = pf::project;

    const auto role = detail::id("key.role");
    const auto function = detail::id("key.function");
    const auto cadence = detail::id("key.cadence");
    const auto boundary = detail::id("key.boundary");
    const auto open = detail::id("boundary.open");
    const auto closed = detail::id("boundary.closed");
    const auto tier = detail::id("tier");
    const auto rule = detail::id("rule");
    Program program{
        {
            detail::id("candidate.stay"),
            {},
            {detail::stage(fixture, "stay", false)},
        },
        {
            detail::id("candidate.travel"),
            {},
            {detail::stage(fixture, "travel", true)},
        },
    };
    auto production = grammar::Term::alt(
        detail::id("production"),
        {
            {
                detail::id("branch.stay"),
                program.stay.cost,
                detail::candidate(program.stay, "stay"),
            },
            {
                detail::id("branch.travel"),
                program.travel.cost,
                detail::candidate(program.travel, "travel"),
            },
        });
    if (!production) {
        return std::unexpected(production.error());
    }

    return Set{
        detail::id("choice"),
        std::move(program),
        std::move(*production),
        {{
            pp::role::Read{role},
            pp::phrase::function::Read{function},
            pp::phrase::cadence::Read{cadence},
            pp::phrase::boundary::Read{boundary, open, closed},
        }},
        {
            {fixture.role.root},
            {tier},
            {{
                {{
                    {role, fixture.role.root},
                    {function, fixture.phrase.function},
                    {cadence, fixture.cadence},
                    {boundary, closed},
                }},
                pf::Aim{
                    tier,
                    {
                        rule,
                        {{fixture.role.root, Rational(1)}},
                        {},
                        Rational(1),
                        "neutral generated root target",
                    },
                },
            }},
            {{detail::id("need"), {rule}}},
        },
    };
}

} // namespace mq::kernel::fixture::generation
