#include "Internal.hpp"

#include <algorithm>
#include <set>

namespace mq::kernel::pitch::tendency::detail {
namespace {

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

std::expected<void, Error> insert(
    std::set<Identity>& identities,
    const Identity& identity) {
    if (!complete(identity)) {
        return std::unexpected(Error{
            Error::Code::Input,
            "tendency input contains an incomplete identity",
            {},
            std::nullopt,
        });
    }
    if (!identities.insert(identity).second) {
        return std::unexpected(Error{
            Error::Code::Input,
            "tendency input repeats identity " + identity.str(),
            {},
            std::nullopt,
        });
    }
    return {};
}

} // namespace

std::expected<Model, Error> model(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::span<const Target> targets) {
    if (targets.empty()) {
        return std::unexpected(Error{
            Error::Code::Input,
            "tendency tier is empty",
            {},
            std::nullopt,
        });
    }

    Model result;
    result.variables.assign(variables.begin(), variables.end());
    result.equations.assign(equations.begin(), equations.end());
    result.inequalities.assign(inequalities.begin(), inequalities.end());
    result.targets.assign(targets.begin(), targets.end());
    std::ranges::sort(result.variables);
    std::ranges::sort(result.equations, {}, &Equation::identity);
    std::ranges::sort(result.inequalities, {}, &Inequality::identity);
    std::ranges::sort(result.targets, {}, &Target::identity);

    std::set<Identity> known;
    std::set<Identity> identities;
    for (const auto& variable : result.variables) {
        if (variable.domain.starts_with("mq.kernel.kkt")) {
            return std::unexpected(Error{
                Error::Code::Input,
                "pitch variable uses the reserved KKT identity domain",
                {},
                std::nullopt,
            });
        }
        const auto added = insert(known, variable);
        if (!added) {
            return std::unexpected(added.error());
        }
        const auto unique = insert(identities, variable);
        if (!unique) {
            return std::unexpected(unique.error());
        }
    }

    for (const auto& equation : result.equations) {
        const auto added = insert(identities, equation.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    for (const auto& inequality : result.inequalities) {
        const auto added = insert(identities, inequality.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
    }

    result.forms.reserve(result.targets.size());
    for (const auto& target : result.targets) {
        const auto added = insert(identities, target.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
        if (target.weight <= Rational(0)) {
            return std::unexpected(Error{
                Error::Code::Input,
                "tendency weight is not positive at " +
                    target.identity.str(),
                {},
                std::nullopt,
            });
        }
        Form form;
        for (const auto& term : target.terms) {
            if (!known.contains(term.variable)) {
                return std::unexpected(Error{
                    Error::Code::Input,
                    "unknown tendency variable " + term.variable.str(),
                    {},
                    std::nullopt,
                });
            }
            form[term.variable] += term.coefficient;
        }
        std::erase_if(
            form,
            [](const auto& item) {
                return item.second == Rational(0);
            });
        result.forms.push_back(std::move(form));
    }
    return result;
}

} // namespace mq::kernel::pitch::tendency::detail
