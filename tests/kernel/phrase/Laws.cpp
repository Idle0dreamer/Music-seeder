#include "../Test.hpp"

#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/fixture/Set.hpp"

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.phrase", std::move(name), "1"};
}

} // namespace

void test::phrase::laws() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    const eval::Evaluator evaluator(
        fixture.profile.shared,
        fixture.catalog);
    state::Snapshot initial;
    initial.jins.active = mq::kernel::sort::JinsId{fixture.jins.root};
    const auto phraseId = id("span");
    const auto eventId = id("event");
    const operation::Begin begin{
        phraseId,
        mq::kernel::phrase::Function{fixture.phrase.function},
    };
    const operation::Place place{
        eventId,
        fixture.role.root,
        motion::Direction::Start,
        fixture.region.root,
        std::nullopt,
    };
    const operation::Cadence cadence{
        fixture.cadence,
        Rational(2),
        Rational(3, 4),
    };
    const operation::End end{
        phraseId,
        mq::kernel::phrase::Boundary::Closed,
    };
    const std::vector<operation::Any> legal{
        begin,
        place,
        cadence,
        end,
    };
    const auto completed = evaluator.run(initial, legal);
    require(completed.has_value(), completed ? "" : completed.error().message);
    require(
        !completed->phrase.active &&
            completed->phrase.completed.size() == 1 &&
            completed->phrase.completed.front().first == eventId &&
            completed->phrase.completed.front().last == eventId &&
            completed->phrase.completed.front().cadences.size() == 1 &&
            completed->phrase.completed.front().cadences.front().event ==
                eventId &&
            completed->phrase.completed.front().cadences.front().strength ==
                Rational(3, 4) &&
            completed->evidence.amount.at(evidence::Kind::Cadence) ==
                Rational(2),
        "phrase span lost event bounds or distinct cadence quantities");
    require(
        completed->trace.events.front().operation == "Begin" &&
            completed->trace.events.back().operation == "End",
        "phrase operations were mislabeled in the trace");

    const auto opened =
        evaluator.run(initial, std::vector<operation::Any>{begin});
    require(opened.has_value(), opened ? "" : opened.error().message);
    const auto nested =
        evaluator.run(*opened, std::vector<operation::Any>{begin});
    require(
        !nested && nested.error().rule == "phrase.nesting",
        "nested phrase was accepted");
    const auto empty =
        evaluator.run(*opened, std::vector<operation::Any>{end});
    require(
        !empty && empty.error().rule == "phrase.empty",
        "empty phrase was accepted");
    const auto contextless =
        evaluator.run({}, std::vector<operation::Any>{cadence});
    require(
        !contextless && contextless.error().rule == "phrase.active",
        "cadence outside a phrase was accepted");

    const auto duplicate =
        evaluator.run(*completed, std::vector<operation::Any>{begin});
    require(
        !duplicate && duplicate.error().rule == "phrase.identity",
        "completed phrase identity was reused");
    const operation::Begin denied{
        id("denied"),
        mq::kernel::phrase::Function{id("unknown-function")},
    };
    const auto forbidden =
        evaluator.run({}, std::vector<operation::Any>{denied});
    require(
        !forbidden &&
            forbidden.error().rule == "allow.phrase.function",
        "profile-disallowed phrase function was accepted");

    const auto first =
        evaluator.run(*opened, std::vector<operation::Any>{place, cadence});
    require(first.has_value(), first ? "" : first.error().message);
    const operation::Place tail{
        id("tail"),
        fixture.role.ghammaz,
        motion::Direction::Rise,
        fixture.region.upper,
        std::nullopt,
    };
    const auto unclosed = evaluator.run(
        *first,
        std::vector<operation::Any>{tail, end});
    require(
        !unclosed && unclosed.error().rule == "phrase.closure",
        "closed phrase accepted a cadence before its final event");
}
