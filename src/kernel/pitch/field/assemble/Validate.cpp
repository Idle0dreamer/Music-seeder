#include "Internal.hpp"

namespace mq::kernel::pitch::field::assemble::detail {

std::expected<Facts, Error> validate(
    const Schema& schema,
    const Context& context,
    Limits limits) {
    if (context.facts.size() > limits.facts ||
        schema.rules.size() > limits.rules ||
        schema.needs.size() > limits.needs) {
        return std::unexpected(Error{
            Error::Code::Count,
            "field assembly budget exceeded",
            std::nullopt,
        });
    }
    if (schema.variables.empty() || schema.rules.empty()) {
        return std::unexpected(input(
            "field requires variables and rules"));
    }

    Facts contextFacts;
    const auto validContext = check(context.facts, &contextFacts);
    if (!validContext) {
        return std::unexpected(validContext.error());
    }

    std::set<Identity> all;
    std::set<Identity> variables;
    std::set<Identity> tiers;
    for (const auto& variable : schema.variables) {
        const auto added = add(all, variable);
        if (!added) {
            return std::unexpected(added.error());
        }
        variables.insert(variable);
    }
    for (const auto& tier : schema.tiers) {
        const auto added = add(all, tier);
        if (!added) {
            return std::unexpected(added.error());
        }
        tiers.insert(tier);
    }

    std::set<Identity> rules;
    for (const auto& rule : schema.rules) {
        const auto valid = check(
            rule,
            variables,
            tiers,
            all,
            rules);
        if (!valid) {
            return std::unexpected(valid.error());
        }
    }
    for (const auto& need : schema.needs) {
        const auto valid = check(need, rules, all);
        if (!valid) {
            return std::unexpected(valid.error());
        }
    }
    return contextFacts;
}

} // namespace mq::kernel::pitch::field::assemble::detail
