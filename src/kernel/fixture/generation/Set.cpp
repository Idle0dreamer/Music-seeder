#include "Internal.hpp"

namespace mq::kernel::fixture::generation {

std::expected<Set, std::string> make(
    const fixture::Set& fixture) {
    const auto key = detail::keys();
    Program program{
        {
            detail::id("candidate.stay"),
            {},
            detail::stages(fixture, "stay", false),
        },
        {
            detail::id("candidate.travel"),
            {},
            detail::stages(fixture, "travel", true),
        },
    };
    auto production = grammar::Term::alt(
        detail::id("production"),
        {
            grammar::Branch{
                detail::id("branch.stay"),
                program.stay.cost,
                detail::candidate(program.stay, "stay"),
            },
            grammar::Branch{
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
        detail::projection(key),
        detail::schema(fixture, key),
    };
}

} // namespace mq::kernel::fixture::generation
