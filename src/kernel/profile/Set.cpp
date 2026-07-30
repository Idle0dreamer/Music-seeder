#include "mq/kernel/profile/Set.hpp"

namespace mq::kernel::profile {

bool Rule::equivalent(const Rule& other) const {
    return value == other.value;
}

Set::Set(std::string identity)
    : identity_(std::move(identity)) {}

const std::string& Set::identity() const noexcept {
    return identity_;
}

const std::map<std::string, Rule>& Set::rules() const noexcept {
    return rules_;
}

const Rule* Set::find(const std::string& key) const noexcept {
    const auto found = rules_.find(key);
    return found == rules_.end() ? nullptr : &found->second;
}

bool Set::allows(
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
Set::parameter(const std::string& key) const {
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

} // namespace mq::kernel::profile
