#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

#include <expected>
#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace mq::kernel::profile {

using Domain = std::set<Identity>;
using Ordering = std::vector<Identity>;
using Value = std::variant<bool, Rational, Domain, Ordering>;

struct Rule {
    Value value;
    std::vector<std::string> provenance;

    [[nodiscard]] bool equivalent(const Rule& other) const;
};

struct Patch {
    enum class Action {
        Define,
        Refine,
        Prefer,
        Parameterize,
        Forbid,
        AddException,
        Resolve,
    };

    Action action;
    std::string key;
    Rule rule;
};

class Set {
public:
    Set() = default;
    explicit Set(std::string identity);

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

    friend std::expected<Set, std::string> reconstruct(
        std::string,
        const std::vector<Set>&,
        const std::vector<Patch>&);
};

[[nodiscard]] std::expected<Set, std::string> reconstruct(
    std::string identity,
    const std::vector<Set>& parents,
    const std::vector<Patch>& patches);

} // namespace mq::kernel::profile
