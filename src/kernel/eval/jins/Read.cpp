#include "Internal.hpp"

namespace mq::kernel::eval {

std::expected<const mq::kernel::jins::Descriptor*, Violation>
Evaluator::descriptor(
    const Identity& identity,
    std::size_t index,
    const char* operation) const {
    if (context_.jins.catalog == nullptr) {
        return std::unexpected(jins::reject(
            index,
            operation,
            "jins.catalog",
            "jins operation requires a descriptor catalog"));
    }
    const auto* found = context_.jins.catalog->find(identity);
    if (found == nullptr) {
        return std::unexpected(jins::reject(
            index,
            operation,
            "jins.descriptor",
            "jins descriptor is missing: " + identity.str()));
    }
    return found;
}

std::expected<const mq::kernel::jins::Descriptor*, Violation>
Evaluator::descriptor(
    const state::Snapshot& state,
    std::size_t index,
    const char* operation) const {
    if (!state.jins.active) {
        return std::unexpected(jins::reject(
            index,
            operation,
            "jins.active",
            "operation requires an active jins descriptor"));
    }
    return descriptor(*state.jins.active.identity, index, operation);
}

} // namespace mq::kernel::eval
