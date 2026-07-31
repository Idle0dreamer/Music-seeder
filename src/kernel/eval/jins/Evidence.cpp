#include "Internal.hpp"

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::emphasize(
    state::Snapshot& state,
    const operation::Emphasize& action,
    std::size_t index) const {
    const auto active = descriptor(state, index, "Emphasize");
    if (!active) {
        return std::unexpected(active.error());
    }
    if (!(*active)->emphasis.contains(action.role.identity)) {
        return std::unexpected(jins::reject(
            index,
            "Emphasize",
            "jins.emphasis",
            "role is not an emphasis role in the active descriptor"));
    }
    if (!profile_.allows("allow.emphasize", action.role.identity)) {
        return std::unexpected(jins::reject(
            index,
            "Emphasize",
            "allow.emphasize",
            "profile rejected emphasis role " + action.role.identity.str()));
    }
    if (action.amount <= Rational(0)) {
        return std::unexpected(jins::reject(
            index,
            "Emphasize",
            "evidence.amount",
            "emphasis evidence must be positive"));
    }
    state.evidence.amount[evidence::Kind::Emphasis] += action.amount;
    return {};
}

std::expected<void, Violation> Evaluator::dwell(
    state::Snapshot& state,
    const operation::Dwell& action,
    std::size_t index) const {
    const auto active = descriptor(state, index, "Dwell");
    if (!active) {
        return std::unexpected(active.error());
    }
    if (!(*active)->roles.contains(action.role.identity)) {
        return std::unexpected(jins::reject(
            index,
            "Dwell",
            "jins.role",
            "dwell role is outside the active descriptor"));
    }
    if (!profile_.allows("allow.dwell", action.role.identity)) {
        return std::unexpected(jins::reject(
            index,
            "Dwell",
            "allow.dwell",
            "profile rejected dwell role " + action.role.identity.str()));
    }
    if (action.amount <= Rational(0)) {
        return std::unexpected(jins::reject(
            index,
            "Dwell",
            "evidence.amount",
            "dwell evidence must be positive"));
    }
    state.evidence.amount[evidence::Kind::Dwell] += action.amount;
    return {};
}

} // namespace mq::kernel::eval
