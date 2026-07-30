#include "Internal.hpp"

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::fulfill(
    state::Snapshot& state,
    const operation::sayr::Fulfill& action,
    std::size_t index) const {
    if (context_.sayr.plan == nullptr) {
        return std::unexpected(sayr::reject(
            index,
            "sayr.plan",
            "sayr fulfillment requires a validated plan"));
    }
    if (!sayr::consistent(state.sayr)) {
        return std::unexpected(sayr::reject(
            index,
            "sayr.state",
            "sayr completion set and history disagree"));
    }
    const auto* obligation =
        context_.sayr.plan->find(action.obligation);
    if (obligation == nullptr) {
        return std::unexpected(sayr::reject(
            index,
            "sayr.obligation",
            "sayr obligation is absent: " +
                action.obligation.str()));
    }
    if (state.sayr.completed.contains(action.obligation)) {
        return std::unexpected(sayr::reject(
            index,
            "sayr.repeat",
            "sayr obligation is already complete"));
    }
    for (const auto& predecessor : obligation->after) {
        if (!state.sayr.completed.contains(predecessor)) {
            return std::unexpected(sayr::reject(
                index,
                "sayr.order",
                "sayr predecessor is incomplete: " +
                    predecessor.str()));
        }
    }

    mq::kernel::sayr::Completion completion{
        action.obligation,
        {},
    };
    for (const auto& need : obligation->needs) {
        auto proof = sayr::prove(need, state, index);
        if (!proof) {
            return std::unexpected(proof.error());
        }
        for (auto& item : *proof) {
            completion.proofs.push_back({
                need.identity,
                std::move(item),
            });
        }
    }
    state.sayr.completed.insert(action.obligation);
    state.sayr.history.push_back(std::move(completion));
    return {};
}

} // namespace mq::kernel::eval
