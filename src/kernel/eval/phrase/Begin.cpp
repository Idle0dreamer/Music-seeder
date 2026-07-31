#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::begin(
    state::Snapshot& state,
    const operation::Begin& action,
    std::size_t index) const {
    if (!phrase::complete(action.phrase.identity) ||
        !phrase::complete(action.function.identity)) {
        return std::unexpected(phrase::reject(
            index,
            "Begin",
            "phrase.identity",
            "phrase and function identities must be complete"));
    }
    if (!profile_.allows(
            "allow.phrase.function",
            action.function.identity)) {
        return std::unexpected(phrase::reject(
            index,
            "Begin",
            "allow.phrase.function",
            "profile rejected phrase function " +
                action.function.identity.str()));
    }
    if (state.phrase.active) {
        return std::unexpected(phrase::reject(
            index,
            "Begin",
            "phrase.nesting",
            "a phrase is already active"));
    }
    const auto repeated = std::ranges::find(
        state.phrase.completed,
        action.phrase.identity,
        &mq::kernel::phrase::Span::identity);
    if (repeated != state.phrase.completed.end()) {
        return std::unexpected(phrase::reject(
            index,
            "Begin",
            "phrase.identity",
            "phrase identity is already completed " +
                action.phrase.str()));
    }

    state.phrase.active = mq::kernel::phrase::Frame{
        action.phrase.identity,
        action.function,
        state.melody.history.size(),
        {},
    };
    return {};
}

} // namespace mq::kernel::eval
