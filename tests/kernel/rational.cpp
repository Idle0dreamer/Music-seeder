#include "Test.hpp"

#include "mq/kernel/Rational.hpp"

#include <limits>
#include <stdexcept>

void test::rational() {
    using mq::kernel::Rational;

    require(Rational(2, 4) == Rational(1, 2), "rational did not normalize");
    require(
        Rational(1, 3) + Rational(1, 6) == Rational(1, 2),
        "rational addition is incorrect");
    require(
        Rational(7, 9) * Rational(27, 14) == Rational(3, 2),
        "cross-cancelled multiplication is incorrect");
    require(
        Rational(-3, -5) == Rational(3, 5),
        "rational sign normalization is incorrect");

    bool minimum = false;
    try {
        static_cast<void>(Rational(
            std::numeric_limits<std::int64_t>::min(),
            1));
    } catch (const std::overflow_error&) {
        minimum = true;
    }
    require(
        minimum,
        "unsafe rational normalization was not rejected");

    bool denominator = false;
    try {
        static_cast<void>(Rational(1, 0));
    } catch (const std::invalid_argument&) {
        denominator = true;
    }
    require(
        denominator,
        "zero rational denominator was not rejected");
}
