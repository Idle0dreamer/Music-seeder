#pragma once

#include <compare>
#include <cstdint>
#include <string>

namespace mq::kernel {

class Rational {
public:
    constexpr Rational() = default;
    Rational(std::int64_t numerator, std::int64_t denominator = 1);

    [[nodiscard]] constexpr std::int64_t numerator() const noexcept {
        return numerator_;
    }

    [[nodiscard]] constexpr std::int64_t denominator() const noexcept {
        return denominator_;
    }

    [[nodiscard]] double decimal() const noexcept;
    [[nodiscard]] std::string str() const;

    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);

    friend Rational operator+(Rational left, const Rational& right) {
        return left += right;
    }

    friend Rational operator-(Rational left, const Rational& right) {
        return left -= right;
    }

    friend Rational operator*(Rational left, const Rational& right) {
        return left *= right;
    }

    friend Rational operator/(Rational left, const Rational& right) {
        return left /= right;
    }

    friend Rational operator-(const Rational& value) {
        return Rational(-value.numerator_, value.denominator_);
    }

    friend constexpr bool operator==(const Rational&, const Rational&) = default;

    friend auto operator<=>(const Rational& left, const Rational& right) {
        const auto diff = left - right;
        if (diff.numerator_ < 0) return std::strong_ordering::less;
        if (diff.numerator_ > 0) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }



private:
    std::int64_t numerator_{};
    std::int64_t denominator_{1};
};

} // namespace mq::kernel
