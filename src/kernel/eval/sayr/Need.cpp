#include "Internal.hpp"

namespace mq::kernel::eval::sayr {

Result prove(
    const mq::kernel::sayr::Need& need,
    const state::Snapshot& state,
    std::size_t index) {
    return std::visit(
        [&](const auto& requirement) {
            return prove(need, requirement, state, index);
        },
        need.requirement);
}

} // namespace mq::kernel::eval::sayr
