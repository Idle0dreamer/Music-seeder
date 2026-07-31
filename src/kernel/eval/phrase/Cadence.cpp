#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::cadence(
    state::Snapshot& state,
    const operation::Cadence& action,
    std::size_t index) const {
    if (!phrase::complete(action.family)) {
        return std::unexpected(phrase::reject(
            index,
            "Cadence",
            "cadence.identity",
            "cadence family identity must be complete"));
    }
    if (!profile_.allows("allow.cadence", action.family.identity)) {
        return std::unexpected(phrase::reject(
            index,
            "Cadence",
            "allow.cadence",
            "profile rejected cadence family " + action.family.str()));
    }
    if (!state.phrase.active) {
        return std::unexpected(phrase::reject(
            index,
            "Cadence",
            "phrase.active",
            "cadence requires an active phrase"));
    }
    if (state.melody.history.size() <= state.phrase.active->first ||
        !state.melody.current) {
        return std::unexpected(phrase::reject(
            index,
            "Cadence",
            "cadence.event",
            "cadence requires a structural event in the active phrase"));
    }
    if (action.evidence <= Rational(0) ||
        action.strength <= Rational(0)) {
        return std::unexpected(phrase::reject(
            index,
            "Cadence",
            "cadence.amount",
            "cadence evidence and strength must be positive"));
    }
    const auto& event = state.melody.current->identity;
    const auto repeated = std::ranges::find(
        state.phrase.active->cadences,
        event,
        &mq::kernel::phrase::Cadence::event);
    if (repeated != state.phrase.active->cadences.end()) {
        return std::unexpected(phrase::reject(
            index,
            "Cadence",
            "cadence.event",
            "structural event already has a cadence"));
    }

    state.phrase.active->cadences.push_back({
        action.family,
        event,
        action.strength,
    });
    state.evidence.amount[evidence::Kind::Cadence] += action.evidence;
    return {};
}

} // namespace mq::kernel::eval
