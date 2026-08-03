#include "Internal.hpp"

#include <set>
#include <variant>

namespace mq::kernel::generate::detail {

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

std::expected<void, Error> check(
    const Identity& choice,
    std::span<const Candidate> candidates,
    Limits limits) {
    if (!complete(choice) || candidates.empty()) {
        return std::unexpected(Error{
            Error::Code::Input,
            "generation choice and candidates must be present",
            {},
            {},
        });
    }
    if (candidates.size() > limits.candidates) {
        return std::unexpected(Error{
            Error::Code::Count,
            "generation candidate budget exceeded",
            {},
            {},
        });
    }

    std::set<Identity> identities;
    for (const auto& candidate : candidates) {
        if (!complete(candidate.identity) ||
            !identities.insert(candidate.identity).second ||
            candidate.stages.empty()) {
            return std::unexpected(Error{
                Error::Code::Input,
                "candidate identity is invalid, repeated, or empty",
                {},
                {},
            });
        }
        if (candidate.stages.size() > limits.stages) {
            return std::unexpected(Error{
                Error::Code::Count,
                "generation stage budget exceeded",
                {},
                {},
            });
        }
        std::set<Identity> stages;
        for (const auto& stage : candidate.stages) {
            std::size_t places{};
            for (const auto& action : stage.actions) {
                places += std::holds_alternative<operation::Place>(action);
            }
            if (!complete(stage.identity) ||
                !stages.insert(stage.identity).second ||
                stage.actions.empty() ||
                places == 0) {
                return std::unexpected(Error{
                    Error::Code::Input,
                    "stage identity or structural-event invariant failed",
                    {},
                    {},
                });
            }
            if (stage.actions.size() > limits.actions) {
                return std::unexpected(Error{
                    Error::Code::Count,
                    "generation action budget exceeded",
                    {},
                    {},
                });
            }
        }
    }
    return {};
}

} // namespace mq::kernel::generate::detail
