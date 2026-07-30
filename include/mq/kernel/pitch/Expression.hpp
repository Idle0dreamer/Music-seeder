#pragma once

#include "mq/kernel/Rational.hpp"

#include <cstdint>
#include <map>
#include <string>

namespace mq::kernel::pitch {

class Expression {
public:
    static Expression cents(Rational value);
    static Expression ratio(std::int64_t numerator, std::int64_t denominator);

    [[nodiscard]] const Rational& offset() const noexcept;
    [[nodiscard]] const std::map<std::int64_t, Rational>& powers() const noexcept;
    [[nodiscard]] double cents() const;
    [[nodiscard]] std::string str() const;

    Expression& operator+=(const Expression& other);
    Expression& operator-=(const Expression& other);
    Expression& operator*=(const Rational& scale);

    friend Expression operator+(Expression left, const Expression& right) {
        return left += right;
    }

    friend Expression operator-(Expression left, const Expression& right) {
        return left -= right;
    }

    friend Expression operator*(Expression left, const Rational& scale) {
        return left *= scale;
    }

    friend bool operator==(const Expression&, const Expression&) = default;

private:
    Rational offset_{};
    std::map<std::int64_t, Rational> powers_;

    void normalize();
};

} // namespace mq::kernel::pitch
