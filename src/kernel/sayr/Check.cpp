#include "Internal.hpp"

#include <type_traits>

namespace mq::kernel::sayr::detail {

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

std::expected<void, std::string> check(
    const Need& item) {
    if (!complete(item.identity)) {
        return std::unexpected("sayr need identity is incomplete");
    }
    const auto valid = std::visit(
        [](const auto& requirement) {
            using Type = std::decay_t<decltype(requirement)>;
            if constexpr (
                std::same_as<Type, need::Jins> ||
                std::same_as<Type, need::Center> ||
                std::same_as<Type, need::Path>) {
                return complete(requirement.identity);
            } else if constexpr (
                std::same_as<Type, need::Role> ||
                std::same_as<Type, need::Region>) {
                return complete(requirement.identity) &&
                       requirement.minimum > 0;
            } else if constexpr (std::same_as<Type, need::Gesture>) {
                return complete(requirement.family) &&
                       requirement.minimum > 0;
            } else if constexpr (std::same_as<Type, need::Phrase>) {
                const bool boundary =
                    requirement.boundary == phrase::Boundary::Open ||
                    requirement.boundary == phrase::Boundary::Closed;
                return complete(requirement.function) &&
                       boundary &&
                       (!requirement.cadence ||
                        complete(*requirement.cadence)) &&
                       requirement.minimum > 0;
            } else {
                return requirement.minimum > Rational(0);
            }
        },
        item.requirement);
    if (!valid) {
        return std::unexpected(
            "sayr need has invalid typed requirement: " +
            item.identity.str());
    }
    return {};
}

} // namespace mq::kernel::sayr::detail
