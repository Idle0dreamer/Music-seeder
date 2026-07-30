#include "Internal.hpp"

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::end(
    state::Snapshot& state,
    const operation::gesture::End& action,
    std::size_t index) const {
    if (!gesture::complete(action.occurrence)) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.End",
            "gesture.identity",
            "gesture occurrence identity must be complete"));
    }
    if (!state.gesture.active) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.End",
            "gesture.active",
            "no gesture occurrence is active"));
    }
    const auto& frame = *state.gesture.active;
    if (frame.occurrence != action.occurrence) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.End",
            "gesture.identity",
            "end identity does not match active gesture"));
    }
    const auto active = descriptor(state, index, "Gesture.End");
    if (!active) {
        return std::unexpected(active.error());
    }
    const auto definition = (*active)->gestures.find(frame.family);
    if (definition == (*active)->gestures.end() ||
        frame.next != definition->second.steps.size()) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.End",
            "gesture.incomplete",
            "gesture ended before every step matched"));
    }
    if (state.melody.history.size() <= frame.first) {
        return std::unexpected(gesture::reject(
            index,
            "Gesture.End",
            "gesture.empty",
            "gesture occurrence has no structural events"));
    }
    state.gesture.completed.push_back({
        frame.occurrence,
        frame.family,
        state.melody.history.at(frame.first).identity,
        state.melody.history.back().identity,
    });
    state.gesture.active.reset();
    return {};
}

} // namespace mq::kernel::eval
