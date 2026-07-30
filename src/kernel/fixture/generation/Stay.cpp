#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

generate::Stage stay(
    const fixture::Set& fixture,
    const std::string& name) {
    const auto phrase = id(name + ".phrase");
    return {
        id(name + ".stage"),
        {
            operation::Anchor{fixture.center.root},
            operation::Enter{fixture.jins.root},
            operation::Begin{
                phrase,
                mq::kernel::phrase::Function{fixture.phrase.function},
            },
            operation::Place{
                id(name + ".event"),
                fixture.role.root,
                motion::Direction::Start,
                fixture.region.root,
                std::nullopt,
            },
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
                fixture.sayr.obligation.settle,
            },
        },
    };
}

} // namespace mq::kernel::fixture::generation::detail
