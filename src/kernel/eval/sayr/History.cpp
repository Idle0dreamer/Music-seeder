#include "Internal.hpp"

namespace mq::kernel::eval::sayr {
namespace {

Result missing(
    const mq::kernel::sayr::Need& source,
    std::size_t index) {
    return std::unexpected(reject(
        index,
        "sayr.need",
        "sayr event-history need is unsatisfied: " +
            source.identity.str()));
}

} // namespace

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Role& need,
    const state::Snapshot& state,
    std::size_t index) {
    Proofs result;
    for (const auto& event : state.melody.history) {
        if (event.role == need.identity) {
            result.push_back(
                mq::kernel::sayr::proof::Role{
                    event.identity,
                    event.role,
                });
        }
    }
    return result.size() < need.minimum
             ? missing(source, index)
             : Result{std::move(result)};
}

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Region& need,
    const state::Snapshot& state,
    std::size_t index) {
    Proofs result;
    for (const auto& event : state.melody.history) {
        if (event.region == need.identity) {
            result.push_back(
                mq::kernel::sayr::proof::Region{
                    event.identity,
                    event.region,
                });
        }
    }
    return result.size() < need.minimum
             ? missing(source, index)
             : Result{std::move(result)};
}

} // namespace mq::kernel::eval::sayr
