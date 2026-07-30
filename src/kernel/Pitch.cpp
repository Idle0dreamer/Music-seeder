#include "mq/kernel/Pitch.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace mq::kernel {
namespace {

void factor(
    std::int64_t value,
    const Rational& sign,
    std::map<std::int64_t, Rational>& powers) {
    if (value <= 0) {
        throw std::invalid_argument("pitch ratio components must be positive");
    }
    for (std::int64_t prime = 2; prime <= value / prime; ++prime) {
        while (value % prime == 0) {
            powers[prime] += sign;
            value /= prime;
        }
    }
    if (value > 1) {
        powers[value] += sign;
    }
}

} // namespace

PitchExpression PitchExpression::cents(Rational value) {
    PitchExpression result;
    result.cents_ = value;
    return result;
}

PitchExpression PitchExpression::ratio(
    std::int64_t numerator,
    std::int64_t denominator) {
    PitchExpression result;
    factor(numerator, Rational(1), result.primePowers_);
    factor(denominator, Rational(-1), result.primePowers_);
    result.normalize();
    return result;
}

const Rational& PitchExpression::centOffset() const noexcept {
    return cents_;
}

const std::map<std::int64_t, Rational>&
PitchExpression::primePowers() const noexcept {
    return primePowers_;
}

double PitchExpression::toCents() const {
    double result = cents_.toDouble();
    for (const auto& [prime, exponent] : primePowers_) {
        result += 1200.0 * exponent.toDouble() *
                  std::log2(static_cast<double>(prime));
    }
    return result;
}

std::string PitchExpression::str() const {
    std::ostringstream output;
    output << cents_.str() << "c";
    for (const auto& [prime, exponent] : primePowers_) {
        output << " + 1200log2(" << prime << ")*" << exponent.str();
    }
    return output.str();
}

PitchExpression& PitchExpression::operator+=(const PitchExpression& other) {
    cents_ += other.cents_;
    for (const auto& [prime, exponent] : other.primePowers_) {
        primePowers_[prime] += exponent;
    }
    normalize();
    return *this;
}

PitchExpression& PitchExpression::operator-=(const PitchExpression& other) {
    cents_ -= other.cents_;
    for (const auto& [prime, exponent] : other.primePowers_) {
        primePowers_[prime] -= exponent;
    }
    normalize();
    return *this;
}

PitchExpression& PitchExpression::operator*=(const Rational& scale) {
    cents_ *= scale;
    for (auto& [prime, exponent] : primePowers_) {
        exponent *= scale;
    }
    normalize();
    return *this;
}

void PitchExpression::normalize() {
    std::erase_if(
        primePowers_,
        [](const auto& item) { return item.second == Rational(0); });
}

void PitchSystem::addVariable(Identity variable) {
    if (std::ranges::find(variables_, variable) != variables_.end()) {
        throw std::invalid_argument("duplicate pitch variable " + variable.str());
    }
    variables_.push_back(std::move(variable));
}

void PitchSystem::addEquation(PitchEquation equation) {
    equations_.push_back(std::move(equation));
}

std::expected<PitchSolution, std::string> PitchSystem::solve() const {
    const auto rowCount = equations_.size();
    const auto columnCount = variables_.size();
    if (rowCount == 0 || columnCount == 0) {
        return std::unexpected("pitch system is empty");
    }

    std::vector<std::vector<Rational>> matrix(
        rowCount,
        std::vector<Rational>(columnCount));
    std::vector<PitchExpression> right;
    right.reserve(rowCount);

    for (std::size_t row = 0; row < rowCount; ++row) {
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

    std::size_t pivotRow = 0;
    std::vector<std::size_t> pivotForColumn(
        columnCount,
        rowCount);
    for (std::size_t column = 0;
         column < columnCount && pivotRow < rowCount;
         ++column) {
        auto selected = pivotRow;
        while (selected < rowCount &&
               matrix[selected][column] == Rational(0)) {
            ++selected;
        }
        if (selected == rowCount) {
            continue;
        }
        std::swap(matrix[pivotRow], matrix[selected]);
        std::swap(right[pivotRow], right[selected]);

        const auto pivot = matrix[pivotRow][column];
        for (std::size_t item = column; item < columnCount; ++item) {
            matrix[pivotRow][item] /= pivot;
        }
        right[pivotRow] *= Rational(1) / pivot;

        for (std::size_t row = 0; row < rowCount; ++row) {
            if (row == pivotRow || matrix[row][column] == Rational(0)) {
                continue;
            }
            const auto multiplier = matrix[row][column];
            for (std::size_t item = column; item < columnCount; ++item) {
                matrix[row][item] -= multiplier * matrix[pivotRow][item];
            }
            right[row] -= right[pivotRow] * multiplier;
        }
        pivotForColumn[column] = pivotRow;
        ++pivotRow;
    }

    for (std::size_t row = 0; row < rowCount; ++row) {
        const auto allZero = std::ranges::all_of(
            matrix[row],
            [](const Rational& value) { return value == Rational(0); });
        if (allZero && right[row] != PitchExpression{}) {
            return std::unexpected(
                "contradictory pitch constraints at " +
                equations_[row].provenance);
        }
    }

    if (std::ranges::find(pivotForColumn, rowCount) !=
        pivotForColumn.end()) {
        return std::unexpected("pitch system is underdetermined");
    }

    PitchSolution solution;
    for (std::size_t column = 0; column < columnCount; ++column) {
        solution.values.emplace(
            variables_[column],
            right[pivotForColumn[column]]);
    }
    return solution;
}

} // namespace mq::kernel
