#include "mq/kernel/eval/Evaluator.hpp"

#include <algorithm>

namespace mq::kernel::eval {
namespace {

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

bool valid(motion::Direction direction) {
    switch (direction) {
    case motion::Direction::Start:
    case motion::Direction::Same:
    case motion::Direction::Rise:
    case motion::Direction::Fall:
        return true;
    }
    return false;
}

Violation reject(
    std::size_t index,
    std::string rule,
    std::string message) {
    return {
        index,
        "Place",
        std::move(rule),
        std::move(message),
    };
}

} // namespace

std::expected<void, Violation> Evaluator::place(
    state::Snapshot& state,
    const operation::Place& event,
    std::size_t index) const {
    if (!complete(event.event) || !complete(event.role)) {
        return std::unexpected(reject(
            index,
            "event.identity",
            "event and role identities must be complete"));
    }
    if (!valid(event.direction)) {
        return std::unexpected(reject(
            index,
            "event.direction",
            "event direction is invalid"));
    }

    const bool empty = state.melody.history.empty();
    const bool consistent =
        empty
            ? !state.melody.current
            : state.melody.current &&
                  *state.melody.current ==
                      state.melody.history.back();
    if (!consistent) {
        return std::unexpected(reject(
            index,
            "event.history",
            "current event and event history disagree"));
    }
    if ((event.direction == motion::Direction::Start) != empty) {
        return std::unexpected(reject(
            index,
            "event.direction",
            empty
                ? "first event must declare Start"
                : "continuing event may not declare Start"));
    }
    if (!profile_.allows("allow.place", event.role)) {
        return std::unexpected(reject(
            index,
            "allow.place",
            "profile rejected structural role " +
                event.role.str()));
    }

    const auto repeated = std::ranges::find(
        state.melody.history,
        event.event,
        &performance::Event::identity);
    if (repeated != state.melody.history.end()) {
        return std::unexpected(reject(
            index,
            "event.identity",
            "event identity is already present " +
                event.event.str()));
    }

    performance::Event placed{
        event.event,
        event.role,
        event.direction,
    };
    state.melody.current = placed;
    state.melody.history.push_back(std::move(placed));
    return {};
}

} // namespace mq::kernel::eval
