#include "Internal.hpp"

namespace mq::kernel::eval::sayr {
namespace {

Result missing(
    const mq::kernel::sayr::Need& source,
    std::size_t index) {
    return std::unexpected(reject(
        index,
        "sayr.need",
        "sayr need is unsatisfied: " + source.identity.str()));
}

} // namespace

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Jins& need,
    const state::Snapshot& state,
    std::size_t index) {
    if (state.jins.active != need.identity) {
        return missing(source, index);
    }
    return Proofs{mq::kernel::sayr::proof::Jins{need.identity}};
}

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Center& need,
    const state::Snapshot& state,
    std::size_t index) {
    if (state.center.stack.empty() ||
        state.center.stack.back() != need.identity) {
        return missing(source, index);
    }
    return Proofs{mq::kernel::sayr::proof::Center{need.identity}};
}

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Path& need,
    const state::Snapshot& state,
    std::size_t index) {
    if (!state.path.completed.contains(need.identity)) {
        return missing(source, index);
    }
    return Proofs{mq::kernel::sayr::proof::Path{need.identity}};
}

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Evidence& need,
    const state::Snapshot& state,
    std::size_t index) {
    const auto found = state.evidence.amount.find(need.kind);
    if (found == state.evidence.amount.end() ||
        found->second < need.minimum) {
        return missing(source, index);
    }
    return Proofs{mq::kernel::sayr::proof::Evidence{
        need.kind,
        found->second,
    }};
}

} // namespace mq::kernel::eval::sayr
