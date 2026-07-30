#include "Internal.hpp"

#include <algorithm>
#include <set>

namespace mq::kernel::pitch::field::assemble {

std::expected<Model, Error> run(
    const Schema& schema,
    const Context& context,
    Limits limits) {
    const auto checked = detail::validate(schema, context, limits);
    if (!checked) {
        return std::unexpected(checked.error());
    }

    std::vector<const Rule*> ordered;
    ordered.reserve(schema.rules.size());
    for (const auto& rule : schema.rules) {
        ordered.push_back(&rule);
    }
    std::ranges::sort(
        ordered,
        {},
        [](const Rule* rule) {
            return detail::identity(*rule);
        });

    Model result;
    result.variables = schema.variables;
    std::ranges::sort(result.variables);
    std::map<Identity, std::vector<tendency::Target>> targets;
    std::set<Identity> active;
    for (const auto* rule : ordered) {
        if (!detail::match(rule->when, *checked)) {
            continue;
        }
        const auto ruleIdentity = detail::identity(*rule);
        active.insert(ruleIdentity);
        result.rules.push_back(ruleIdentity);
        std::visit(
            [&](const auto& effect) {
                using Type = std::decay_t<decltype(effect)>;
                if constexpr (std::same_as<Type, Equation>) {
                    result.equations.push_back(effect);
                } else if constexpr (std::same_as<Type, Inequality>) {
                    result.inequalities.push_back(effect);
                } else {
                    targets[effect.tier].push_back(effect.target);
                }
            },
            rule->effect);
    }

    for (const auto& need : schema.needs) {
        const bool covered = std::ranges::any_of(
            need.any,
            [&active](const Identity& rule) {
                return active.contains(rule);
            });
        if (!covered) {
            return std::unexpected(Error{
                Error::Code::Coverage,
                "field context does not cover " + need.identity.str(),
                need.identity,
            });
        }
    }

    for (const auto& tier : schema.tiers) {
        auto found = targets.find(tier);
        if (found != targets.end() && !found->second.empty()) {
            result.tiers.push_back({
                tier,
                std::move(found->second),
            });
        }
    }
    return result;
}

} // namespace mq::kernel::pitch::field::assemble
