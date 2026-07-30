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

std::expected<feasibility::Report, feasibility::Error> System::feasible(
    feasibility::Limits limits) const {
    return feasibility::check(
        variables_,
        equations_,
        inequalities_,
        limits);
}

std::expected<Solution, std::string> System::solve() const {
    const auto checked = feasible();
    if (!checked) {
        return std::unexpected(
            "pitch feasibility failed: " + checked.error().message);
    }
    if (checked->status == feasibility::Status::Infeasible) {
        const auto source = checked->provenance.empty()
                              ? std::string{"unknown provenance"}
                              : checked->provenance.front();
        return std::unexpected(
            "contradictory hard pitch constraints at " + source);
    }

    const auto rows = equations_.size();
    const auto columns = variables_.size();
    if (rows == 0 || columns == 0) {
        return std::unexpected("pitch system is empty");
    }

    std::vector<std::vector<Rational>> matrix(
        rows,
        std::vector<Rational>(columns));
    std::vector<Expression> right;
    right.reserve(rows);

    for (std::size_t row = 0; row < rows; ++row) {
        for (const auto& term : equations_[row].terms) {
            const auto found = std::ranges::find(variables_, term.variable);
            if (found == variables_.end()) {
                return std::unexpected(
                    "unknown pitch variable " + term.variable.str());
            }
            const auto column =
                static_cast<std::size_t>(std::distance(variables_.begin(), found));
            matrix[row][column] += term.coefficient;
        }
        right.push_back(equations_[row].right);
    }

    std::size_t pivot = 0;
    std::vector<std::size_t> pivots(columns, rows);
    for (std::size_t column = 0;
         column < columns && pivot < rows;
         ++column) {
        auto selected = pivot;
        while (selected < rows && matrix[selected][column] == Rational(0)) {
            ++selected;
        }
        if (selected == rows) {
            continue;
        }
        std::swap(matrix[pivot], matrix[selected]);
        std::swap(right[pivot], right[selected]);

        const auto divisor = matrix[pivot][column];
        for (std::size_t item = column; item < columns; ++item) {
            matrix[pivot][item] /= divisor;
        }
        right[pivot] *= Rational(1) / divisor;

        for (std::size_t row = 0; row < rows; ++row) {
            if (row == pivot || matrix[row][column] == Rational(0)) {
                continue;
            }
            const auto multiplier = matrix[row][column];
            for (std::size_t item = column; item < columns; ++item) {
                matrix[row][item] -= multiplier * matrix[pivot][item];
            }
            right[row] -= right[pivot] * multiplier;
        }
        pivots[column] = pivot;
        ++pivot;
    }

    for (std::size_t row = 0; row < rows; ++row) {
        const auto zero = std::ranges::all_of(
            matrix[row],
            [](const Rational& value) { return value == Rational(0); });
        if (zero && right[row] != Expression{}) {
            return std::unexpected(
                "contradictory pitch constraints at " +
                equations_[row].provenance);
        }
    }

    if (std::ranges::find(pivots, rows) != pivots.end()) {
        return std::unexpected("pitch system is underdetermined");
    }

    Solution solution;
    for (std::size_t column = 0; column < columns; ++column) {
        solution.values.emplace(variables_[column], right[pivots[column]]);
    }
    return solution;
}

} // namespace mq::kernel::pitch
