#include "Internal.hpp"

#include <algorithm>
#include <type_traits>

namespace mq::kernel::sayr::detail {
namespace {

bool matches(
    const Requirement& requirement,
    const proof::Item& value) {
    return std::visit(
        [](const auto& request, const auto& witness) {
            using Need = std::decay_t<decltype(request)>;
            using Proof = std::decay_t<decltype(witness)>;
            if constexpr (
                std::same_as<Need, need::Jins> &&
                std::same_as<Proof, proof::Jins>) {
                return request.identity == witness.identity;
            } else if constexpr (
                std::same_as<Need, need::Center> &&
                std::same_as<Proof, proof::Center>) {
                return request.identity == witness.identity;
            } else if constexpr (
                std::same_as<Need, need::Role> &&
                std::same_as<Proof, proof::Role>) {
                return request.identity == witness.role &&
                       complete(witness.event);
            } else if constexpr (
                std::same_as<Need, need::Region> &&
                std::same_as<Proof, proof::Region>) {
                return request.identity == witness.region &&
                       complete(witness.event);
            } else if constexpr (
                std::same_as<Need, need::Gesture> &&
                std::same_as<Proof, proof::Gesture>) {
                return request.family == witness.family &&
                       complete(witness.occurrence);
            } else if constexpr (
                std::same_as<Need, need::Phrase> &&
                std::same_as<Proof, proof::Phrase>) {
                return request.function == witness.function &&
                       request.boundary == witness.boundary &&
                       (!request.cadence ||
                        request.cadence == witness.cadence) &&
                       complete(witness.phrase);
            } else if constexpr (
                std::same_as<Need, need::Path> &&
                std::same_as<Proof, proof::Path>) {
                return request.identity == witness.identity;
            } else if constexpr (
                std::same_as<Need, need::Evidence> &&
                std::same_as<Proof, proof::Evidence>) {
                return request.kind == witness.kind &&
                       witness.amount >= request.minimum;
            } else {
                return false;
            }
        },
        requirement,
        value);
}

std::size_t minimum(
    const Requirement& requirement) {
    return std::visit(
        [](const auto& request) -> std::size_t {
            using Type = std::decay_t<decltype(request)>;
            if constexpr (
                std::same_as<Type, need::Role> ||
                std::same_as<Type, need::Region> ||
                std::same_as<Type, need::Gesture> ||
                std::same_as<Type, need::Phrase>) {
                return request.minimum;
            }
            return 1;
        },
        requirement);
}

} // namespace

bool valid(
    const Obligation& obligation,
    const Completion& completion) {
    if (completion.obligation.identity != obligation.identity) {
        return false;
    }
    for (const auto& need : obligation.needs) {
        const auto count = std::ranges::count_if(
            completion.proofs,
            [&](const auto& proof) {
                return proof.need == need.identity &&
                       matches(need.requirement, proof.value);
            });
        if (count < static_cast<std::ptrdiff_t>(
                        minimum(need.requirement))) {
            return false;
        }
    }
    return std::ranges::all_of(
        completion.proofs,
        [&](const auto& proof) {
            const auto found = std::ranges::find(
                obligation.needs,
                proof.need,
                &Need::identity);
            return found != obligation.needs.end() &&
                   matches(found->requirement, proof.value);
        });
}

} // namespace mq::kernel::sayr::detail
