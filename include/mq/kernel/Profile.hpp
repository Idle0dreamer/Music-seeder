#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

#include <expected>
#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace mq::kernel {

using Domain = std::set<Identity>;
using Ordering = std::vector<Identity>;
using RuleValue = std::variant<bool, Rational, Domain, Ordering>;

struct Rule {
    RuleValue value;
    std::vector<std::string> provenance;

    [[nodiscard]] bool sameValue(const Rule& other) const;
};

enum class PatchAction {
    Define,
    Refine,
    Prefer,
    Parameterize,
    Forbid,
    AddException,
    Resolve,
};

struct Patch {
    PatchAction action;
    std::string key;
    Rule rule;
};

class Profile {
public:
    Profile() = default;
    explicit Profile(std::string identity);

    [[nodiscard]] const std::string& identity() const noexcept;
    [[nodiscard]] const std::map<std::string, Rule>& rules() const noexcept;
    [[nodiscard]] const Rule* find(const std::string& key) const noexcept;
    [[nodiscard]] bool allows(
        const std::string& key,
        const Identity& value) const noexcept;
    [[nodiscard]] std::expected<Rational, std::string> parameter(
        const std::string& key) const;

private:
    std::string identity_;
    std::map<std::string, Rule> rules_;

    friend std::expected<Profile, std::string> reconstruct(
        std::string,
        const std::vector<Profile>&,
        const std::vector<Patch>&);
};

[[nodiscard]] std::expected<Profile, std::string> reconstruct(
    std::string identity,
    const std::vector<Profile>& parents,
    const std::vector<Patch>& patches);

} // namespace mq::kernel

