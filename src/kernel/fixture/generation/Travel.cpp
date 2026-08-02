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
                operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}},
                operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
                operation::Begin{mq::kernel::sort::PhraseId{phrase},
                    mq::kernel::phrase::Function{
                        fixture.phrase.function,
                    },
                },
                operation::gesture::Begin{
                    ascent,
                    fixture.gesture.ascent,
                },
                operation::Place{
                    mq::kernel::sort::EventId{id(name + ".event.1")},
                    mq::kernel::sort::RoleId{fixture.role.root},
                    motion::Direction::Start,
                    mq::kernel::sort::RegionId{fixture.region.root},
                    std::nullopt,
                },
                operation::Emit{
                    mq::kernel::sort::CellId{fixture.cell},
                    std::nullopt,
                },
            },
        },
        {
            id(name + ".stage.2"),
            {
                operation::Place{
                    mq::kernel::sort::EventId{id(name + ".event.2")},
                    mq::kernel::sort::RoleId{fixture.role.ghammaz},
                    motion::Direction::Rise,
                    mq::kernel::sort::RegionId{fixture.region.upper},
                    std::nullopt,
                },
                operation::Emphasize{mq::kernel::sort::RoleId{fixture.role.ghammaz},
                    Rational(3),
                },
                operation::Dwell{mq::kernel::sort::RoleId{fixture.role.ghammaz},
                    Rational(2),
                },
                operation::Emit{
                    mq::kernel::sort::CellId{fixture.cell},
                    std::nullopt,
                },
            },
        },
        {
            id(name + ".stage.3"),
            {
                operation::Place{
                    mq::kernel::sort::EventId{id(name + ".event.3")},
                    mq::kernel::sort::RoleId{fixture.role.baggage},
                    motion::Direction::Rise,
                    mq::kernel::sort::RegionId{fixture.region.upper},
                    mq::kernel::sort::BaggageId{fixture.baggage},
                },
                operation::Emit{
                    mq::kernel::sort::CellId{fixture.cell},
                    std::nullopt,
                },
                operation::gesture::End{ascent},
                operation::Cadence{mq::kernel::sort::FamilyId{fixture.cadence},
                    Rational(1),
                    Rational(1),
                },
                operation::End{mq::kernel::sort::PhraseId{phrase},
                    mq::kernel::phrase::Boundary::Closed,
                },
                operation::sayr::Fulfill{mq::kernel::sort::ObligationId{fixture.sayr.obligation.establish}},
                operation::sayr::Fulfill{mq::kernel::sort::ObligationId{fixture.sayr.obligation.expand}},
                operation::sayr::Fulfill{mq::kernel::sort::ObligationId{fixture.sayr.obligation.climax}},
                operation::Tonicize{mq::kernel::sort::JinsId{fixture.jins.branch},
                    tonicization::Level::Internal,
                },
                operation::Modulate{mq::kernel::sort::PathId{fixture.path.direct}, mq::kernel::sort::CenterId{fixture.center.branch}, 
                    tonicization::Level::Internal,
                },
                operation::sayr::Fulfill{mq::kernel::sort::ObligationId{fixture.sayr.obligation.travel}},
            },
        },
        {
            id(name + ".stage.4"),
            {
                operation::Begin{mq::kernel::sort::PhraseId{returning},
                    mq::kernel::phrase::Function{
                        fixture.phrase.function,
                    },
                },
                operation::gesture::Begin{
                    descent,
                    fixture.gesture.descent,
                },
                operation::Place{
                    mq::kernel::sort::EventId{id(name + ".event.4")},
                    mq::kernel::sort::RoleId{fixture.role.ghammaz},
                    motion::Direction::Fall,
                    mq::kernel::sort::RegionId{fixture.region.upper},
                    std::nullopt,
                },
                operation::Emit{
                    mq::kernel::sort::CellId{fixture.cell},
                    std::nullopt,
                },
            },
        },
        {
            id(name + ".stage.5"),
            {
                operation::Place{
                    mq::kernel::sort::EventId{id(name + ".event.5")},
                    mq::kernel::sort::RoleId{fixture.role.root},
                    motion::Direction::Fall,
                    mq::kernel::sort::RegionId{fixture.region.root},
                    std::nullopt,
                },
                operation::Emit{
                    mq::kernel::sort::CellId{fixture.cell},
                    std::nullopt,
                },
                operation::gesture::End{descent},
                operation::Cadence{mq::kernel::sort::FamilyId{fixture.cadence},
                    Rational(1),
                    Rational(1),
                },
                operation::End{mq::kernel::sort::PhraseId{returning},
                    mq::kernel::phrase::Boundary::Closed,
                },
                operation::Return{mq::kernel::sort::CenterId{fixture.center.root}},
                operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
                operation::sayr::Fulfill{mq::kernel::sort::ObligationId{fixture.sayr.obligation.restore}},
            },
        },
    };
}

} // namespace mq::kernel::fixture::generation::detail
