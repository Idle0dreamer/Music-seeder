#include "Internal.hpp"

namespace mq::kernel::eval::place {
namespace {

std::expected<void, Violation> baggage(
    const operation::Place& event,
    const mq::kernel::jins::Descriptor& descriptor,
    const state::Snapshot& state,
    std::size_t index) {
    if (!event.baggage) {
        return {};
    }
    const auto found = descriptor.baggage.find((*event.baggage).identity);
    if (found == descriptor.baggage.end() ||
        found->second.role != event.role.identity ||
        !found->second.regions.contains(event.region.identity) ||
        !found->second.directions.contains(event.direction)) {
        return std::unexpected(reject(
            index,
            "jins.baggage",
            "event does not match its baggage capability"));
    }
    if (!found->second.gestures.empty() &&
        (!state.gesture.active ||
         !found->second.gestures.contains(
             state.gesture.active->family))) {
        return std::unexpected(reject(
            index,
            "jins.baggage",
            "baggage requires a compatible active gesture"));
    }
    return {};
}

} // namespace

std::expected<std::optional<performance::Gesture>, Violation> match(
    const operation::Place& event,
    const mq::kernel::jins::Descriptor& descriptor,
    state::Snapshot& state,
    std::size_t index) {
    const auto capability = baggage(event, descriptor, state, index);
    if (!capability) {
        return std::unexpected(capability.error());
    }
    if (!state.gesture.active) {
        return std::optional<performance::Gesture>{};
    }
    auto& frame = *state.gesture.active;
    const auto found = descriptor.gestures.find(frame.family);
    if (found == descriptor.gestures.end() ||
        frame.next >= found->second.steps.size()) {
        return std::unexpected(reject(
            index,
            "gesture.complete",
            "gesture has no remaining step for this event"));
    }
    const auto& step = found->second.steps[frame.next];
    if (!step.roles.contains(event.role.identity) ||
        !step.regions.contains(event.region.identity) ||
        !step.directions.contains(event.direction) ||
        (step.baggage && event.baggage != step.baggage)) {
        return std::unexpected(reject(
            index,
            "gesture.step",
            "event does not match the next gesture step"));
    }
    ++frame.next;
    return std::optional<performance::Gesture>{{
        frame.occurrence,
        frame.family,
    }};
}

} // namespace mq::kernel::eval::place
