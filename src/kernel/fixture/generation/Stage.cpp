#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

Identity id(std::string name) {
    return {"fixture.generate", std::move(name), "1"};
}

generate::Stage stage(
    const fixture::Set& fixture,
    std::string name,
    bool travel) {
    const auto phraseId = id(name + ".phrase");
    std::vector<operation::Any> actions{
        operation::Anchor{fixture.center.root},
        operation::Enter{fixture.jins.root},
    };
    if (travel) {
        actions.push_back(
            operation::Emphasize{fixture.role.ghammaz, Rational(3)});
        actions.push_back(
            operation::Dwell{fixture.role.ghammaz, Rational(2)});
        actions.push_back(operation::Emit{fixture.cell});
    }
    actions.push_back(operation::Begin{
        phraseId,
        phrase::Function{fixture.phrase.function},
    });
    actions.push_back(operation::Place{
        id(name + ".event"),
        fixture.role.root,
        motion::Direction::Start,
    });
    actions.push_back(operation::Cadence{
        fixture.cadence,
        Rational(1),
        Rational(1),
    });
    actions.push_back(operation::End{
        phraseId,
        phrase::Boundary::Closed,
    });
    if (travel) {
        actions.push_back(operation::Tonicize{
            fixture.jins.branch,
            tonicization::Level::Internal,
        });
        actions.push_back(operation::Modulate{
            fixture.path.direct,
            fixture.center.branch,
            tonicization::Level::Internal,
        });
    }
    return {id(name + ".stage"), std::move(actions)};
}

} // namespace mq::kernel::fixture::generation::detail
