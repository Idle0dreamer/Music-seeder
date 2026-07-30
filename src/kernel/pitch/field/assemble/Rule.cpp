#include "Internal.hpp"

#include <algorithm>
#include <concepts>
#include <type_traits>

namespace mq::kernel::pitch::field::assemble::detail {
namespace {

bool known(
    std::span<const Term> terms,
    const std::set<Identity>& variables) {
    return std::ranges::all_of(
        terms,
        [&variables](const Term& term) {
            return variables.contains(term.variable);
        });
}

} // namespace

std::expected<void, Error> check(
    const Rule& rule,
    const std::set<Identity>& variables,
    const std::set<Identity>& tiers,
    std::set<Identity>& all,
    std::set<Identity>& rules) {
    const auto validGuard = check(rule.when.all);
    if (!validGuard) {
        return std::unexpected(validGuard.error());
    }
    const auto ruleIdentity = identity(rule);
    const auto added = add(all, ruleIdentity);
    if (!added) {
        return std::unexpected(added.error());
    }
    rules.insert(ruleIdentity);

    return std::visit(
        [&](const auto& effect) -> std::expected<void, Error> {
            using Type = std::decay_t<decltype(effect)>;
            const auto& terms = [&]() -> const std::vector<Term>& {
                if constexpr (std::same_as<Type, Aim>) {
                    return effect.target.terms;
                } else {
                    return effect.terms;
                }
            }();
            if (!known(terms, variables)) {
                return std::unexpected(input(
                    "field rule uses an unknown variable",
                    ruleIdentity));
            }
            const auto& provenance = [&]() -> const std::string& {
                if constexpr (std::same_as<Type, Aim>) {
                    return effect.target.provenance;
                } else {
                    return effect.provenance;
                }
            }();
            if (provenance.empty()) {
                return std::unexpected(input(
                    "field rule lacks provenance",
                    ruleIdentity));
            }
            if constexpr (std::same_as<Type, Aim>) {
                if (!tiers.contains(effect.tier)) {
                    return std::unexpected(input(
                        "field target names an unknown tier",
                        ruleIdentity));
                }
                if (effect.target.weight <= Rational(0)) {
                    return std::unexpected(input(
                        "field target weight is not positive",
                        ruleIdentity));
                }
            }
            return {};
        },
        rule.effect);
}

} // namespace mq::kernel::pitch::field::assemble::detail
