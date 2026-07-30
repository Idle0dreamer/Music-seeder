#include "Internal.hpp"

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::enter(
    state::Snapshot& state,
    const operation::Enter& action,
    std::size_t index) const {
    if (!jins::complete(action.jins)) {
        return std::unexpected(jins::reject(
            index,
            "Enter",
            "jins.identity",
            "jins identity must be complete"));
    }
    if (!profile_.allows("allow.enter", action.jins)) {
        return std::unexpected(jins::reject(
            index,
            "Enter",
            "allow.enter",
            "profile rejected jins " + action.jins.str()));
    }
    if (state.gesture.active) {
        return std::unexpected(jins::reject(
            index,
            "Enter",
            "gesture.active",
            "active gesture must end before entering another jins"));
    }
    const auto found = descriptor(action.jins, index, "Enter");
    if (!found) {
        return std::unexpected(found.error());
    }
    state.jins.active = (*found)->identity;
    return {};
}

} // namespace mq::kernel::eval
