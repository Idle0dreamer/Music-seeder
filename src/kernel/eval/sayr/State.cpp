#include "Internal.hpp"

namespace mq::kernel::eval::sayr {

Violation reject(
    std::size_t index,
    std::string rule,
    std::string message) {
    return {
        index,
        "Sayr.Fulfill",
        std::move(rule),
        std::move(message),
    };
}

bool consistent(
    const state::Sayr& state) {
    std::set<sort::ObligationId> history;
    for (const auto& completion : state.history) {
        if (!history.insert(completion.obligation).second) {
            return false;
        }
    }
    return history == state.completed;
}

} // namespace mq::kernel::eval::sayr
