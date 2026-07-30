#include "Test.hpp"

#include "mq/kernel/pitch/System.hpp"

#include <cmath>

namespace {

mq::kernel::pitch::Solution solve(
    const mq::kernel::pitch::Expression& root) {
    using namespace mq::kernel;
    const Identity tonic{"test.pitch", "tonic", "1"};
    const Identity second{"test.pitch", "second", "1"};
    const Identity fourth{"test.pitch", "fourth", "1"};

    pitch::System system;
    system.declare(tonic);
    system.declare(second);
    system.declare(fourth);
    system.equate({{{tonic, Rational(1)}}, root, "root anchor"});
    system.equate({
        {{fourth, Rational(1)}, {tonic, Rational(-1)}},
        pitch::Expression::ratio(4, 3),
        "4:3 structural closure",
    });
    system.equate({
        {{second, Rational(1)}, {tonic, Rational(-1)}},
        pitch::Expression::ratio(9, 8) +
            pitch::Expression::cents(Rational(-9, 2)),
        "contextual second",
    });
    const auto solved = system.solve();
    require(solved.has_value(), solved.error_or("pitch solve failed"));
    return *solved;
}

} // namespace

void test::pitch() {
    using namespace mq::kernel;
    const Identity tonic{"test.pitch", "tonic", "1"};
    const Identity second{"test.pitch", "second", "1"};
    const Identity fourth{"test.pitch", "fourth", "1"};

    const auto base = solve(pitch::Expression::cents(Rational(0)));
    require(
        base.values.at(fourth) - base.values.at(tonic) ==
            pitch::Expression::ratio(4, 3),
        "ratio identity was rounded or lost");
    require(
        base.values.at(second).offset() == Rational(-9, 2),
        "contextual cents term was not preserved exactly");

    const auto shifted = solve(pitch::Expression::cents(Rational(125, 2)));
    require(
        shifted.values.at(fourth) - base.values.at(fourth) ==
            pitch::Expression::cents(Rational(125, 2)),
        "pitch translation law failed");

    const auto cents =
        (base.values.at(fourth) - base.values.at(tonic)).cents();
    require(
        std::abs(cents - 498.044999) < 0.001,
        "symbolic ratio realization is numerically wrong");

    pitch::System contradiction;
    contradiction.declare(tonic);
    contradiction.equate({
        {{tonic, Rational(1)}},
        pitch::Expression::cents(Rational(0)),
        "first anchor",
    });
    contradiction.equate({
        {{tonic, Rational(1)}},
        pitch::Expression::cents(Rational(1)),
        "contradictory anchor",
    });
    require(
        !contradiction.solve().has_value(),
        "contradictory hard pitch constraints were averaged");
}
