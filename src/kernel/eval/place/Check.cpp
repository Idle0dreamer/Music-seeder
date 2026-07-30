#include "Internal.hpp"

namespace mq::kernel::eval::place {
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

} // namespace

Violation reject(
    std::size_t index,
    std::string rule,
    std::string message) {
    return {index, "Place", std::move(rule), std::move(message)};
}

std::expected<void, Violation> check(
    const operation::Place& event,
    const mq::kernel::jins::Descriptor& descriptor,
    const profile::Set& profile,
    const state::Snapshot& state,
    std::size_t index) {
    if (!complete(event.event) ||
        !complete(event.role) ||
        !complete(event.region)) {
        return std::unexpected(reject(
            index,
            "event.identity",
            "event, role, and register identities must be complete"));
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
                  *state.melody.current == state.melody.history.back();
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
    if (!descriptor.roles.contains(event.role)) {
        return std::unexpected(reject(
            index,
            "jins.role",
            "structural role is outside the active descriptor"));
    }
    if (!descriptor.regions.contains(event.region)) {
        return std::unexpected(reject(
            index,
            "jins.register",
            "register region is outside the active descriptor"));
    }
    if (!profile.allows("allow.place", event.role)) {
        return std::unexpected(reject(
            index,
            "allow.place",
            "profile rejected structural role " + event.role.str()));
    }
    if (!profile.allows("allow.register", event.region)) {
        return std::unexpected(reject(
            index,
            "allow.register",
            "profile rejected register region " + event.region.str()));
    }
    return {};
}

} // namespace mq::kernel::eval::place
