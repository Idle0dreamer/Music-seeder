#include "mq/kernel/pitch/System.hpp"

#include <algorithm>
#include <stdexcept>

namespace mq::kernel::pitch {

void System::declare(Identity variable) {
    if (std::ranges::find(variables_, variable) != variables_.end()) {
        throw std::invalid_argument("duplicate pitch variable " + variable.str());
    }
    variables_.push_back(std::move(variable));
}

void System::equate(Equation equation) {
    equations_.push_back(std::move(equation));
}

void System::bound(Inequality inequality) {
    inequalities_.push_back(std::move(inequality));
}

std::expected<feasibility::Result, feasibility::Error> System::feasible(
    feasibility::Limits limits) const {
    return feasibility::analyze(
        variables_,
        equations_,
        inequalities_,
        limits);
}

std::expected<Solution, std::string> System::solve(
    feasibility::Limits limits) const {
    auto checked = feasible(limits);
    if (!checked) {
        return std::unexpected(
            "pitch feasibility failed: " + checked.error().message);
    }
    if (checked->status == feasibility::Status::Infeasible ||
        !checked->solution) {
        const auto source = checked->provenance.empty()
                              ? std::string{"unknown provenance"}
                              : checked->provenance.front();
        return std::unexpected(
            "contradictory hard pitch constraints at " + source);
    }

    return std::move(*checked->solution);
}

} // namespace mq::kernel::pitch
