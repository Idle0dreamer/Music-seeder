#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::begin(
    state::Snapshot& state,
    const operation::gesture::Begin& action,
    std::size_t index) const {
    if (!gesture::complete(action.occurrence) ||
        !gesture::complete(action.family)) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.Begin",
            "gesture.identity",
            "gesture occurrence and family identities must be complete"));
    }
    const auto active = descriptor(state, index, "Gesture.Begin");
    if (!active) {
        return std::unexpected(active.error());
    }
    if (!(*active)->gestures.contains(action.family)) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.Begin",
            "jins.gesture",
            "gesture family is outside the active descriptor"));
    }
    if (!profile_.allows("allow.gesture", action.family)) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.Begin",
            "allow.gesture",
            "profile rejected gesture family " + action.family.str()));
    }
    if (state.gesture.active) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.Begin",
            "gesture.nesting",
            "a gesture occurrence is already active"));
    }
    const auto repeated = std::ranges::find(
        state.gesture.completed,
        action.occurrence,
        &mq::kernel::gesture::Span::occurrence);
    if (repeated != state.gesture.completed.end()) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.Begin",
            "gesture.identity",
            "gesture occurrence identity is already completed"));
    }
    state.gesture.active = mq::kernel::gesture::Frame{
        action.occurrence,
        action.family,
        state.melody.history.size(),
        0,
    };
    return {};
}

} // namespace mq::kernel::eval
