#include "mq/kernel/profile/Set.hpp"

#include <algorithm>
#include <sstream>

namespace mq::kernel::profile {
namespace {

bool subset(const Domain& candidate, const Domain& existing) {
    return std::ranges::all_of(
        candidate,
        [&](const Identity& value) { return existing.contains(value); });
}

void append(Rule& target, const Rule& source) {
    for (const auto& item : source.provenance) {
        if (std::ranges::find(target.provenance, item) ==
            target.provenance.end()) {
            target.provenance.push_back(item);
        }
    }
}

} // namespace

std::expected<Set, std::string> reconstruct(
    std::string identity,
    const std::vector<Set>& parents,
    const std::vector<Patch>& patches) {
    Set result(std::move(identity));
    std::set<std::string> conflicts;

    for (const auto& parent : parents) {
        for (const auto& [key, incoming] : parent.rules_) {
            const auto found = result.rules_.find(key);
            if (found == result.rules_.end()) {
                result.rules_.emplace(key, incoming);
            } else if (found->second.equivalent(incoming)) {
                append(found->second, incoming);
            } else {
                conflicts.insert(key);
            }
        }
    }

    for (const auto& patch : patches) {
        const auto found = result.rules_.find(patch.key);
        switch (patch.action) {
        case Patch::Action::Define:
            if (found != result.rules_.end() || conflicts.contains(patch.key)) {
                return std::unexpected(
                    "Define requires an absent key: " + patch.key);
            }
            result.rules_.emplace(patch.key, patch.rule);
            break;

        case Patch::Action::Refine: {
            if (found == result.rules_.end() ||
                conflicts.contains(patch.key)) {
                return std::unexpected(
                    "Refine requires one resolved parent rule: " + patch.key);
            }
            const auto* current = std::get_if<Domain>(&found->second.value);
            const auto* refined = std::get_if<Domain>(&patch.rule.value);
            if (current == nullptr || refined == nullptr ||
                !subset(*refined, *current)) {
                return std::unexpected(
                    "Refine may only narrow a domain: " + patch.key);
            }
            found->second.value = *refined;
            append(found->second, patch.rule);
            break;
        }

        case Patch::Action::Prefer:
            if (!std::holds_alternative<Ordering>(patch.rule.value)) {
                return std::unexpected(
                    "Prefer requires an ordering: " + patch.key);
            }
            result.rules_[patch.key] = patch.rule;
            conflicts.erase(patch.key);
            break;

        case Patch::Action::Parameterize:
            if (!std::holds_alternative<Rational>(patch.rule.value)) {
                return std::unexpected(
                    "Parameterize requires a rational: " + patch.key);
            }
            result.rules_[patch.key] = patch.rule;
            conflicts.erase(patch.key);
            break;

        case Patch::Action::Forbid:
            if (found == result.rules_.end()) {
                return std::unexpected(
                    "Forbid requires an existing rule: " + patch.key);
            }
            result.rules_[patch.key] =
                Rule{false, patch.rule.provenance};
            conflicts.erase(patch.key);
            break;

        case Patch::Action::AddException: {
            if (found == result.rules_.end() ||
                conflicts.contains(patch.key)) {
                return std::unexpected(
                    "AddException requires one resolved domain: " + patch.key);
            }
            auto* current = std::get_if<Domain>(&found->second.value);
            const auto* additions = std::get_if<Domain>(&patch.rule.value);
            if (current == nullptr || additions == nullptr) {
                return std::unexpected(
                    "AddException requires domains: " + patch.key);
            }
            current->insert(additions->begin(), additions->end());
            append(found->second, patch.rule);
            break;
        }

        case Patch::Action::Resolve:
            if (!conflicts.contains(patch.key)) {
                return std::unexpected(
                    "Resolve requires a parent conflict: " + patch.key);
            }
            result.rules_[patch.key] = patch.rule;
            conflicts.erase(patch.key);
            break;
        }
    }

    if (!conflicts.empty()) {
        std::ostringstream message;
        message << "unresolved profile conflicts:";
        for (const auto& key : conflicts) {
            message << ' ' << key;
        }
        return std::unexpected(message.str());
    }
    return result;
}

} // namespace mq::kernel::profile
