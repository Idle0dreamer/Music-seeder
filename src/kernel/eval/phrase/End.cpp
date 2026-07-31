#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::end(
    state::Snapshot& state,
    const operation::End& action,
    std::size_t index) const {
    if (!phrase::complete(action.phrase.identity)) {
        return std::unexpected(phrase::reject(
            index,
            "End",
            "phrase.identity",
            "phrase identity must be complete"));
    }
    if (!state.phrase.active) {
        return std::unexpected(phrase::reject(
            index,
            "End",
            "phrase.active",
            "no phrase is active"));
    }
    const auto& frame = *state.phrase.active;
    if (frame.identity != action.phrase.identity) {
        return std::unexpected(phrase::reject(
            index,
            "End",
            "phrase.identity",
            "end identity does not match the active phrase"));
    }
    if (state.melody.history.size() <= frame.first) {
        return std::unexpected(phrase::reject(
            index,
            "End",
            "phrase.empty",
            "phrase has no structural events"));
    }

    const auto& first = state.melody.history.at(frame.first).identity;
    const auto& last = state.melody.history.back().identity;
    if (action.boundary == mq::kernel::phrase::Boundary::Closed) {
        const auto closure = std::ranges::find(
            frame.cadences,
            last,
            &mq::kernel::phrase::Cadence::event);
        if (closure == frame.cadences.end()) {
            return std::unexpected(phrase::reject(
                index,
                "End",
                "phrase.closure",
                "closed phrase requires a cadence on its final event"));
        }
    }

    state.phrase.completed.push_back({
        frame.identity,
        frame.function,
        first,
        last,
        action.boundary,
        frame.cadences,
    });
    state.phrase.active.reset();
    return {};
}

} // namespace mq::kernel::eval
