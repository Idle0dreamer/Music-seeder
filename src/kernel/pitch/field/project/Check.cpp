#include "Internal.hpp"

#include <array>
#include <type_traits>

namespace mq::kernel::pitch::field::project::detail {

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

const Identity& key(const Source& source) {
    return std::visit(
        [](const auto& read) -> const Identity& {
            return read.key;
        },
        source);
}

std::expected<void, Error> check(
    const Plan& plan,
    Limits limits) {
    if (plan.sources.empty()) {
        return std::unexpected(Error{
            Error::Code::Input,
            "field projection plan is empty",
            std::nullopt,
        });
    }
    if (plan.sources.size() > limits.sources) {
        return std::unexpected(Error{
            Error::Code::Count,
            "field projection source budget exceeded",
            std::nullopt,
        });
    }

    std::set<Identity> keys;
    for (const auto& source : plan.sources) {
        const auto& sourceKey = key(source);
        if (!complete(sourceKey) || !keys.insert(sourceKey).second) {
            return std::unexpected(Error{
                Error::Code::Input,
                "field projection has an invalid or repeated key",
                sourceKey,
            });
        }
        const auto valid = std::visit(
            [&](const auto& read) {
                using Type = std::decay_t<decltype(read)>;
                if constexpr (std::same_as<Type, tonicization::Read>) {
                    return complete(read.color) &&
                           complete(read.internal) &&
                           complete(read.maqam);
                } else if constexpr (std::same_as<Type, path::Read>) {
                    return complete(read.path) &&
                           complete(read.complete) &&
                           complete(read.incomplete) &&
                           read.complete != read.incomplete;
                } else {
                    return true;
                }
            },
            source);
        if (!valid) {
            return std::unexpected(Error{
                Error::Code::Input,
                "field projection source has invalid mapped identities",
                sourceKey,
            });
        }
    }
    return {};
}

} // namespace mq::kernel::pitch::field::project::detail
