#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::sayr {

std::expected<Plan, std::string> Plan::make(
    Identity identity,
    std::vector<Obligation> obligations,
    std::vector<Route> routes) {
    if (!detail::complete(identity) ||
        obligations.empty() ||
        routes.empty()) {
        return std::unexpected(
            "sayr plan identity, obligations, or routes are invalid");
    }
    Plan result;
    result.identity_ = std::move(identity);
    std::set<Identity> needs;
    for (auto& obligation : obligations) {
        if (!detail::complete(obligation.identity) ||
            obligation.needs.empty() ||
            result.obligations_.contains(obligation.identity)) {
            return std::unexpected(
                "sayr obligation identity or needs are invalid");
        }
        for (const auto& need : obligation.needs) {
            const auto valid = detail::check(need);
            if (!valid) {
                return std::unexpected(valid.error());
            }
            if (!needs.insert(need.identity).second) {
                return std::unexpected(
                    "sayr need identity is duplicated: " +
                    need.identity.str());
            }
        }
        std::ranges::sort(
            obligation.needs,
            {},
            &Need::identity);
        result.obligations_.emplace(
            obligation.identity,
            std::move(obligation));
    }
    for (const auto& [identity, obligation] : result.obligations_) {
        if (obligation.after.contains(identity)) {
            return std::unexpected(
                "sayr obligation depends on itself: " + identity.str());
        }
        for (const auto& predecessor : obligation.after) {
            if (!result.obligations_.contains(predecessor)) {
                return std::unexpected(
                    "sayr predecessor is missing: " +
                    predecessor.str());
            }
        }
    }
    const auto ordered = detail::acyclic(result.obligations_);
    if (!ordered) {
        return std::unexpected(ordered.error());
    }
    for (auto& route : routes) {
        if (!detail::complete(route.identity) ||
            route.terminals.empty() ||
            result.routes_.contains(route.identity)) {
            return std::unexpected(
                "sayr route identity or terminals are invalid");
        }
        for (const auto& terminal : route.terminals) {
            if (!result.obligations_.contains(terminal)) {
                return std::unexpected(
                    "sayr route terminal is missing: " +
                    terminal.str());
            }
        }
        result.routes_.emplace(route.identity, std::move(route));
    }
    return result;
}

const Identity& Plan::identity() const noexcept {
    return identity_;
}

const Obligation* Plan::find(
    const Identity& identity) const noexcept {
    const auto found = obligations_.find(identity);
    return found == obligations_.end() ? nullptr : &found->second;
}

bool Plan::accepts(
    const std::vector<Completion>& history) const {
    std::set<Identity> completed;
    for (const auto& completion : history) {
        const auto* obligation = find(completion.obligation);
        if (obligation == nullptr ||
            completed.contains(completion.obligation) ||
            !detail::valid(*obligation, completion) ||
            !std::ranges::all_of(
                obligation->after,
                [&](const auto& predecessor) {
                    return completed.contains(predecessor);
                })) {
            return false;
        }
        completed.insert(completion.obligation);
    }
    return std::ranges::any_of(
        routes_,
        [&](const auto& item) {
            return std::ranges::all_of(
                item.second.terminals,
                [&](const auto& terminal) {
                    return completed.contains(terminal);
                });
        });
}

std::size_t Plan::size() const noexcept {
    return obligations_.size();
}

} // namespace mq::kernel::sayr
