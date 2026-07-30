#pragma once

#include "mq/kernel/pitch/Feasibility.hpp"
#include "mq/kernel/pitch/Solution.hpp"

#include <expected>
#include <string>
#include <vector>

namespace mq::kernel::pitch {

class System {
public:
    void declare(Identity variable);
    void equate(Equation equation);
    void bound(Inequality inequality);

    [[nodiscard]] std::expected<Solution, std::string> solve(
        feasibility::Limits limits = {}) const;
    [[nodiscard]] std::expected<feasibility::Result, feasibility::Error>
    feasible(feasibility::Limits limits = {}) const;

private:
    std::vector<Identity> variables_;
    std::vector<Equation> equations_;
    std::vector<Inequality> inequalities_;
};

} // namespace mq::kernel::pitch
