#include "mq/kernel/Profile.hpp"

#include <algorithm>
#include <sstream>

namespace mq::kernel {
namespace {

bool isSubset(const Domain& candidate, const Domain& existing) {
    return std::ranges::all_of(
        candidate,
        [&](const Identity& value) { return existing.contains(value); });
}

void appendProvenance(Rule& target, const Rule& source) {
    for (const auto& item : source.provenance) {
        if (std::ranges::find(target.provenance, item) ==
            target.provenance.end()) {
            target.provenance.push_back(item);
        }
    }
}

} // namespace

bool Rule::sameValue(const Rule& other) const {
    return value == other.value;
}

Profile::Profile(std::string identity)
    : identity_(std::move(identity)) {}

const std::string& Profile::identity() const noexcept {
    return identity_;
}

const std::map<std::string, Rule>& Profile::rules() const noexcept {
    return rules_;
}

const Rule* Profile::find(const std::string& key) const noexcept {
    const auto found = rules_.find(key);
    return found == rules_.end() ? nullptr : &found->second;
}

bool Profile::allows(
    const std::string& key,
    const Identity& value) const noexcept {
    const auto* rule = find(key);
    if (rule == nullptr) {
        return false;
    }
    if (const auto* domain = std::get_if<Domain>(&rule->value)) {
        return domain->contains(value);
    }
    if (const auto* flag = std::get_if<bool>(&rule->value)) {
        return *flag;
    }
    return false;
}

std::expected<Rational, std::string>
Profile::parameter(const std::string& key) const {
    const auto* rule = find(key);
    if (rule == nullptr) {
        return std::unexpected("missing profile parameter " + key);
    }
    const auto* value = std::get_if<Rational>(&rule->value);
    if (value == nullptr) {
        return std::unexpected("profile rule is not a parameter: " + key);
    }
    return *value;
}

std::expected<Profile, std::string> reconstruct(
    std::string identity,
    const std::vector<Profile>& parents,
    const std::vector<Patch>& patches) {
    Profile result(std::move(identity));
    std::set<std::string> conflicts;

    for (const auto& parent : parents) {
        for (const auto& [key, incoming] : parent.rules_) {
            const auto found = result.rules_.find(key);
            if (found == result.rules_.end()) {
                result.rules_.emplace(key, incoming);
            } else if (found->second.sameValue(incoming)) {
                appendProvenance(found->second, incoming);
            } else {
                conflicts.insert(key);
            }
        }
    }

    for (const auto& patch : patches) {
        const auto found = result.rules_.find(patch.key);
        switch (patch.action) {
        case PatchAction::Define:
            if (found != result.rules_.end() || conflicts.contains(patch.key)) {
                return std::unexpected(
                    "Define requires an absent key: " + patch.key);
            }
            result.rules_.emplace(patch.key, patch.rule);
            break;

        case PatchAction::Refine: {
            if (found == result.rules_.end() ||
                conflicts.contains(patch.key)) {
                return std::unexpected(
                    "Refine requires one resolved parent rule: " + patch.key);
            }
            const auto* current = std::get_if<Domain>(&found->second.value);
            const auto* refined = std::get_if<Domain>(&patch.rule.value);
            if (current == nullptr || refined == nullptr ||
                !isSubset(*refined, *current)) {
                return std::unexpected(
                    "Refine may only narrow a domain: " + patch.key);
            }
            found->second.value = *refined;
            appendProvenance(found->second, patch.rule);
            break;
        }

        case PatchAction::Prefer:
            if (!std::holds_alternative<Ordering>(patch.rule.value)) {
                return std::unexpected(
                    "Prefer requires an ordering: " + patch.key);
            }
            result.rules_[patch.key] = patch.rule;
            conflicts.erase(patch.key);
            break;

        case PatchAction::Parameterize:
            if (!std::holds_alternative<Rational>(patch.rule.value)) {
                return std::unexpected(
                    "Parameterize requires a rational: " + patch.key);
            }
            result.rules_[patch.key] = patch.rule;
            conflicts.erase(patch.key);
            break;

        case PatchAction::Forbid:
            if (found == result.rules_.end()) {
                return std::unexpected(
                    "Forbid requires an existing rule: " + patch.key);
            }
            result.rules_[patch.key] =
                Rule{false, patch.rule.provenance};
            conflicts.erase(patch.key);
            break;

        case PatchAction::AddException: {
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
            appendProvenance(found->second, patch.rule);
            break;
        }

        case PatchAction::Resolve:
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

} // namespace mq::kernel
