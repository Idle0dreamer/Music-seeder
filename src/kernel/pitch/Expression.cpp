#include "mq/kernel/pitch/Expression.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

namespace mq::kernel::pitch {
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

Expression Expression::cents(Rational value) {
    Expression result;
    result.offset_ = value;
    return result;
}

Expression Expression::ratio(
    std::int64_t numerator,
    std::int64_t denominator) {
    Expression result;
    factor(numerator, Rational(1), result.powers_);
    factor(denominator, Rational(-1), result.powers_);
    result.normalize();
    return result;
}

const Rational& Expression::offset() const noexcept {
    return offset_;
}

const std::map<std::int64_t, Rational>& Expression::powers() const noexcept {
    return powers_;
}

double Expression::cents() const {
    double result = offset_.decimal();
    for (const auto& [prime, exponent] : powers_) {
        result += 1200.0 * exponent.decimal() *
                  std::log2(static_cast<double>(prime));
    }
    return result;
}

std::string Expression::str() const {
    std::ostringstream output;
    output << offset_.str() << "c";
    for (const auto& [prime, exponent] : powers_) {
        output << " + 1200log2(" << prime << ")*" << exponent.str();
    }
    return output.str();
}

Expression& Expression::operator+=(const Expression& other) {
    offset_ += other.offset_;
    for (const auto& [prime, exponent] : other.powers_) {
        powers_[prime] += exponent;
    }
    normalize();
    return *this;
}

Expression& Expression::operator-=(const Expression& other) {
    offset_ -= other.offset_;
    for (const auto& [prime, exponent] : other.powers_) {
        powers_[prime] -= exponent;
    }
    normalize();
    return *this;
}

Expression& Expression::operator*=(const Rational& scale) {
    offset_ *= scale;
    for (auto& [prime, exponent] : powers_) {
        exponent *= scale;
    }
    normalize();
    return *this;
}

void Expression::normalize() {
    std::erase_if(
        powers_,
        [](const auto& item) { return item.second == Rational(0); });
}

} // namespace mq::kernel::pitch
