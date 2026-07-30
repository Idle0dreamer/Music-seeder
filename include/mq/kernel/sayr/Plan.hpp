#pragma once

#include "mq/kernel/sayr/Completion.hpp"
#include "mq/kernel/sayr/Obligation.hpp"
#include "mq/kernel/sayr/Route.hpp"

#include <expected>
#include <map>
#include <set>
#include <string>

namespace mq::kernel::sayr {

class Plan {
public:
    Plan() = default;

    [[nodiscard]] static std::expected<Plan, std::string> make(
        Identity identity,
        std::vector<Obligation> obligations,
        std::vector<Route> routes);

    [[nodiscard]] const Identity& identity() const noexcept;
    [[nodiscard]] const Obligation* find(
        const Identity& identity) const noexcept;
    [[nodiscard]] bool accepts(
        const std::vector<Completion>& history) const;
    [[nodiscard]] std::size_t size() const noexcept;

private:
    Identity identity_;
    std::map<Identity, Obligation> obligations_;
    std::map<Identity, Route> routes_;
};

} // namespace mq::kernel::sayr
