#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

std::vector<generate::Stage> travel(
    const fixture::Set& fixture,
    const std::string& name) {
    const auto phrase = id(name + ".phrase");
    const auto ascent = id(name + ".gesture.ascent");
    const auto descent = id(name + ".gesture.descent");
    const auto returning = id(name + ".phrase.return");
    return {
        {
            id(name + ".stage.1"),
            {
                operation::Anchor{fixture.center.root},
                operation::Enter{fixture.jins.root},
                operation::Begin{
                    phrase,
                    mq::kernel::phrase::Function{
                        fixture.phrase.function,
                    },
                },
                operation::gesture::Begin{
                    ascent,
                    fixture.gesture.ascent,
                },
                operation::Place{
                    id(name + ".event.1"),
                    fixture.role.root,
                    motion::Direction::Start,
                    fixture.region.root,
                    std::nullopt,
                },
            },
        },
        {
            id(name + ".stage.2"),
            {
                operation::Place{
                    id(name + ".event.2"),
                    fixture.role.ghammaz,
                    motion::Direction::Rise,
                    fixture.region.upper,
                    std::nullopt,
                },
                operation::Emphasize{
                    fixture.role.ghammaz,
                    Rational(3),
                },
                operation::Dwell{
                    fixture.role.ghammaz,
                    Rational(2),
                },
                operation::Emit{fixture.cell},
            },
        },
        {
            id(name + ".stage.3"),
            {
                operation::Place{
                    id(name + ".event.3"),
                    fixture.role.baggage,
                    motion::Direction::Rise,
                    fixture.region.upper,
                    fixture.baggage,
                },
                operation::gesture::End{ascent},
                operation::Cadence{
                    fixture.cadence,
                    Rational(1),
                    Rational(1),
                },
                operation::End{
                    phrase,
                    mq::kernel::phrase::Boundary::Closed,
                },
                operation::sayr::Fulfill{
                    fixture.sayr.obligation.establish,
                },
                operation::sayr::Fulfill{
                    fixture.sayr.obligation.expand,
                },
                operation::sayr::Fulfill{
                    fixture.sayr.obligation.climax,
                },
                operation::Tonicize{
                    fixture.jins.branch,
                    tonicization::Level::Internal,
                },
                operation::Modulate{
                    fixture.path.direct,
                    fixture.center.branch,
                    tonicization::Level::Internal,
                },
                operation::sayr::Fulfill{
                    fixture.sayr.obligation.travel,
                },
            },
        },
        {
            id(name + ".stage.4"),
            {
                operation::Begin{
                    returning,
                    mq::kernel::phrase::Function{
                        fixture.phrase.function,
                    },
                },
                operation::gesture::Begin{
                    descent,
                    fixture.gesture.descent,
                },
                operation::Place{
                    id(name + ".event.4"),
                    fixture.role.ghammaz,
                    motion::Direction::Fall,
                    fixture.region.upper,
                    std::nullopt,
                },
            },
        },
        {
            id(name + ".stage.5"),
            {
                operation::Place{
                    id(name + ".event.5"),
                    fixture.role.root,
                    motion::Direction::Fall,
                    fixture.region.root,
                    std::nullopt,
                },
                operation::gesture::End{descent},
                operation::Cadence{
                    fixture.cadence,
                    Rational(1),
                    Rational(1),
                },
                operation::End{
                    returning,
                    mq::kernel::phrase::Boundary::Closed,
                },
                operation::Return{fixture.center.root},
                operation::Enter{fixture.jins.root},
                operation::sayr::Fulfill{
                    fixture.sayr.obligation.restore,
                },
            },
        },
    };
}

} // namespace mq::kernel::fixture::generation::detail
