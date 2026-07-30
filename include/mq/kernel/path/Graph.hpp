#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/tonicization/Level.hpp"

#include <expected>
#include <map>
#include <set>
#include <string>

namespace mq::kernel::path {

struct Rule {
    Identity identity;
    Identity source;
    Identity target;
    tonicization::Level maximum{tonicization::Level::Internal};
    std::set<Identity> prior;
    std::string provenance;
};

class Graph {
public:
    [[nodiscard]] std::expected<void, std::string> add(Rule rule);
    [[nodiscard]] const Rule* find(const Identity& path) const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;

private:
    std::map<Identity, Rule> rules_;
};

} // namespace mq::kernel::path
