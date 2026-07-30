#include "Internal.hpp"

#include <set>

namespace mq::kernel::pitch::tendency::sequence::detail {
namespace {

std::expected<void, Error> insert(
    std::set<Identity>& identities,
    const Identity& identity) {
    if (identity.domain.empty() ||
        identity.name.empty() ||
        identity.revision.empty()) {
        return std::unexpected(Error{
            Error::Code::Input,
            "tendency sequence contains an incomplete identity",
            std::nullopt,
        });
    }
    if (identity.domain.starts_with("mq.kernel.kkt") ||
        identity.domain.starts_with("mq.kernel.sequence")) {
        return std::unexpected(Error{
            Error::Code::Input,
            "tendency sequence uses a reserved identity domain",
            std::nullopt,
        });
    }
    if (!identities.insert(identity).second) {
        return std::unexpected(Error{
            Error::Code::Input,
            "tendency sequence repeats identity " + identity.str(),
            std::nullopt,
        });
    }
    return {};
}

} // namespace

std::expected<void, Error> validate(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Tier> tiers,
    std::size_t limit) {
    if (tiers.empty()) {
        return std::unexpected(Error{
            Error::Code::Input,
            "tendency sequence is empty",
            std::nullopt,
        });
    }
    if (tiers.size() > limit) {
        return std::unexpected(Error{
            Error::Code::Count,
            "tendency tier-count budget exceeded",
            std::nullopt,
        });
    }

    std::set<Identity> identities;
    for (const auto& variable : variables) {
        const auto added = insert(identities, variable);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    for (const auto& equation : equations) {
        const auto added = insert(identities, equation.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    for (const auto& inequality : inequalities) {
        const auto added = insert(identities, inequality.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    for (const auto& tier : tiers) {
        const auto added = insert(identities, tier.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
        if (tier.targets.empty()) {
            return std::unexpected(Error{
                Error::Code::Input,
                "tendency tier is empty at " + tier.identity.str(),
                std::nullopt,
            });
        }
        for (const auto& target : tier.targets) {
            const auto targetAdded = insert(identities, target.identity);
            if (!targetAdded) {
                return std::unexpected(targetAdded.error());
            }
        }
    }
    return {};
}

} // namespace mq::kernel::pitch::tendency::sequence::detail
