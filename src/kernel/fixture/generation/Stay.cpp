#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

generate::Stage stay(
    const fixture::Set& fixture,
    const std::string& name) {
    const auto phrase = id(name + ".phrase");
    return {
        id(name + ".stage"),
        {
            operation::Anchor{mq::kernel::sort::CenterId{fixture.center.root}},
            operation::Enter{mq::kernel::sort::JinsId{fixture.jins.root}},
            operation::Begin{mq::kernel::sort::PhraseId{phrase},
                mq::kernel::phrase::Function{fixture.phrase.function},
            },
            operation::Place{
                    mq::kernel::sort::EventId{id(name + ".event")},
                    mq::kernel::sort::RoleId{fixture.role.root},
                    motion::Direction::Start,
                    mq::kernel::sort::RegionId{fixture.region.root},
                    std::nullopt,
                },
            operation::Cadence{mq::kernel::sort::FamilyId{fixture.cadence},
                Rational(1),
                Rational(1),
            },
            operation::End{mq::kernel::sort::PhraseId{phrase},
                mq::kernel::phrase::Boundary::Closed,
            },
            operation::sayr::Fulfill{mq::kernel::sort::ObligationId{fixture.sayr.obligation.establish}},
            operation::sayr::Fulfill{mq::kernel::sort::ObligationId{fixture.sayr.obligation.settle}},
        },
    };
}

} // namespace mq::kernel::fixture::generation::detail
