#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::eval::sayr {
namespace {

Result missing(
    const mq::kernel::sayr::Need& source,
    std::size_t index) {
    return std::unexpected(reject(
        index,
        "sayr.need",
        "sayr span need is unsatisfied: " + source.identity.str()));
}

} // namespace

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Gesture& need,
    const state::Snapshot& state,
    std::size_t index) {
    Proofs result;
    for (const auto& span : state.gesture.completed) {
        if (span.family == need.family) {
            result.push_back(
                mq::kernel::sayr::proof::Gesture{
                    span.occurrence,
                    span.family,
                });
        }
    }
    return result.size() < need.minimum
             ? missing(source, index)
             : Result{std::move(result)};
}

Result prove(
    const mq::kernel::sayr::Need& source,
    const mq::kernel::sayr::need::Phrase& need,
    const state::Snapshot& state,
    std::size_t index) {
    Proofs result;
    for (const auto& span : state.phrase.completed) {
        if (span.function.identity != need.function ||
            span.boundary != need.boundary) {
            continue;
        }
        std::optional<Identity> cadence;
        if (need.cadence) {
            const auto found = std::ranges::find(
                span.cadences,
                *need.cadence,
                &phrase::Cadence::family);
            if (found == span.cadences.end()) {
                continue;
            }
            cadence = found->family;
        }
        result.push_back(
            mq::kernel::sayr::proof::Phrase{
                span.identity,
                span.function.identity,
                span.boundary,
                cadence,
            });
    }
    return result.size() < need.minimum
             ? missing(source, index)
             : Result{std::move(result)};
}

} // namespace mq::kernel::eval::sayr
