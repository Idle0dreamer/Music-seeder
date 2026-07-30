#include "Internal.hpp"

#include <set>

namespace mq::kernel::pitch::feasibility::detail {
namespace {

std::expected<void, Error> insert(
    std::set<Identity>& identities,
    const Identity& identity,
    std::string kind) {
    if (identity.domain.empty() ||
        identity.name.empty() ||
        identity.revision.empty()) {
        return std::unexpected(Error{
            Error::Code::Input,
            "pitch " + kind + " identity is incomplete",
            std::nullopt,
        });
    }
    if (!identities.insert(identity).second) {
        return std::unexpected(Error{
            Error::Code::Input,
            "duplicate pitch identity " + identity.str(),
            std::nullopt,
        });
    }
    return {};
}

} // namespace

std::expected<void, Error> identities(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities) {
    std::set<Identity> seen;
    for (const auto& variable : variables) {
        const auto added = insert(seen, variable, "variable");
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    for (const auto& equation : equations) {
        const auto added = insert(seen, equation.identity, "constraint");
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    for (const auto& inequality : inequalities) {
        const auto added = insert(seen, inequality.identity, "constraint");
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    return {};
}

} // namespace mq::kernel::pitch::feasibility::detail
