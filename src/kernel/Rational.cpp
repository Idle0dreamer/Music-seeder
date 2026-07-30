#include "mq/kernel/Rational.hpp"

#include <cstdlib>
#include <limits>
#include <numeric>
#include <stdexcept>

namespace mq::kernel {
namespace {

std::int64_t checkedAdd(std::int64_t left, std::int64_t right) {
    std::int64_t result{};
    if (__builtin_add_overflow(left, right, &result)) {
        throw std::overflow_error("rational addition overflow");
    }
    return result;
}

std::int64_t checkedMultiply(std::int64_t left, std::int64_t right) {
    std::int64_t result{};
    if (__builtin_mul_overflow(left, right, &result)) {
        throw std::overflow_error("rational multiplication overflow");
    }
    return result;
}

} // namespace

Rational::Rational(std::int64_t numerator, std::int64_t denominator) {
    if (denominator == 0) {
        throw std::invalid_argument("rational denominator is zero");
    }
    if (numerator == std::numeric_limits<std::int64_t>::min() ||
        denominator == std::numeric_limits<std::int64_t>::min()) {
        throw std::overflow_error(
            "rational component cannot be normalized safely");
    }
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
    const auto divisor = std::gcd(numerator, denominator);
    numerator_ = numerator / divisor;
    denominator_ = denominator / divisor;
}

double Rational::decimal() const noexcept {
    return static_cast<double>(numerator_) /
           static_cast<double>(denominator_);
}

std::string Rational::str() const {
    if (denominator_ == 1) {
        return std::to_string(numerator_);
    }
    return std::to_string(numerator_) + "/" + std::to_string(denominator_);
}

Rational& Rational::operator+=(const Rational& other) {
    const auto common = std::gcd(denominator_, other.denominator_);
    const auto leftScale = other.denominator_ / common;
    const auto rightScale = denominator_ / common;
    const auto left = checkedMultiply(numerator_, leftScale);
    const auto right = checkedMultiply(other.numerator_, rightScale);
    const auto denominator = checkedMultiply(denominator_, leftScale);
    *this = Rational(checkedAdd(left, right), denominator);
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    return *this += -other;
}

Rational& Rational::operator*=(const Rational& other) {
    const auto cancelLeft =
        std::gcd(std::abs(numerator_), other.denominator_);
    const auto cancelRight =
        std::gcd(std::abs(other.numerator_), denominator_);
    const auto leftNumerator = numerator_ / cancelLeft;
    const auto rightNumerator = other.numerator_ / cancelRight;
    const auto leftDenominator = denominator_ / cancelRight;
    const auto rightDenominator = other.denominator_ / cancelLeft;
    *this = Rational(
        checkedMultiply(leftNumerator, rightNumerator),
        checkedMultiply(leftDenominator, rightDenominator));
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    if (other.numerator_ == 0) {
        throw std::invalid_argument("division by zero rational");
    }
    return *this *= Rational(other.denominator_, other.numerator_);
}

} // namespace mq::kernel
