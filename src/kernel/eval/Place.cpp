#include "mq/kernel/eval/Evaluator.hpp"

#include "place/Internal.hpp"

#include <algorithm>

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::place(
    state::Snapshot& state,
    const operation::Place& event,
    std::size_t index) const {
    const auto active = descriptor(state, index, "Place");
    if (!active) {
        return std::unexpected(active.error());
    }
    const auto valid =
        place::check(event, **active, profile_, state, index);
    if (!valid) {
        return std::unexpected(valid.error());
    }
    const auto repeated = std::ranges::find(
        state.melody.history,
        event.event.identity,
        &performance::Event::identity);
    if (repeated != state.melody.history.end()) {
        return std::unexpected(place::reject(
            index,
            "event.identity",
            "event identity is already present " + event.event.str()));
    }
    const auto gesture = place::match(
        event,
        **active,
        state,
        index);
    if (!gesture) {
        return std::unexpected(gesture.error());
    }
    if (event.baggage &&
        !profile_.allows("allow.baggage", event.baggage->identity)) {
        return std::unexpected(place::reject(
            index,
            "allow.baggage",
            "profile rejected baggage " + event.baggage->str()));
    }

    performance::Event placed{
        event.event.identity,
        event.role.identity,
        event.direction,
        event.region.identity,
        event.baggage ? std::optional(event.baggage->identity) : std::nullopt,
        *gesture,
    };
    state.melody.current = placed;
    state.melody.history.push_back(std::move(placed));
    if (event.baggage) {
        state.evidence.amount[evidence::Kind::Baggage] += Rational(1);
    }
    if ((*active)->characteristic.contains(event.region.identity)) {
        state.evidence.amount[evidence::Kind::Register] += Rational(1);
    }
    return {};
}

} // namespace mq::kernel::eval
