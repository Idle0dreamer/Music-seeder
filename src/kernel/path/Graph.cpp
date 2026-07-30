#include "mq/kernel/path/Graph.hpp"

namespace mq::kernel::path {

std::expected<void, std::string> Graph::add(Rule rule) {
    if (rule.provenance.empty()) {
        return std::unexpected(
            "path rule requires provenance: " + rule.identity.str());
    }
    if (rule.source == rule.target) {
        return std::unexpected(
            "path endpoints must differ: " + rule.identity.str());
    }
    const auto inserted = rules_.emplace(rule.identity, std::move(rule)).second;
    if (!inserted) {
        return std::unexpected("duplicate path rule");
    }
    return {};
}

const Rule* Graph::find(const Identity& path) const noexcept {
    const auto found = rules_.find(path);
    return found == rules_.end() ? nullptr : &found->second;
}

std::size_t Graph::size() const noexcept {
    return rules_.size();
}

} // namespace mq::kernel::path
