#include "Test.hpp"

#include "mq/kernel/Pitch.hpp"

#include <cmath>

namespace {

mq::kernel::PitchSolution solvedField(const mq::kernel::PitchExpression& root) {
    using namespace mq::kernel;
    const Identity tonic{"test.pitch", "tonic", "1"};
    const Identity second{"test.pitch", "second", "1"};
    const Identity fourth{"test.pitch", "fourth", "1"};

    PitchSystem system;
    system.addVariable(tonic);
    system.addVariable(second);
    system.addVariable(fourth);
    system.addEquation({{{tonic, Rational(1)}}, root, "root anchor"});
    system.addEquation({
        {{fourth, Rational(1)}, {tonic, Rational(-1)}},
        PitchExpression::ratio(4, 3),
        "4:3 structural closure",
    });
    system.addEquation({
        {{second, Rational(1)}, {tonic, Rational(-1)}},
        PitchExpression::ratio(9, 8) +
            PitchExpression::cents(Rational(-9, 2)),
        "contextual second",
    });
    const auto solved = system.solve();
    require(solved.has_value(), solved.error_or("pitch solve failed"));
    return *solved;
}

} // namespace

void pitchTests() {
    using namespace mq::kernel;
    const Identity tonic{"test.pitch", "tonic", "1"};
    const Identity second{"test.pitch", "second", "1"};
    const Identity fourth{"test.pitch", "fourth", "1"};

    const auto base = solvedField(PitchExpression::cents(Rational(0)));
    require(
        base.values.at(fourth) - base.values.at(tonic) ==
            PitchExpression::ratio(4, 3),
        "ratio identity was rounded or lost");
    require(
        base.values.at(second).centOffset() == Rational(-9, 2),
        "contextual cents term was not preserved exactly");

    const auto shifted = solvedField(PitchExpression::cents(Rational(125, 2)));
    require(
        shifted.values.at(fourth) - base.values.at(fourth) ==
            PitchExpression::cents(Rational(125, 2)),
        "pitch translation law failed");

    const auto fourthCents =
        (base.values.at(fourth) - base.values.at(tonic)).toCents();
    require(
        std::abs(fourthCents - 498.044999) < 0.001,
        "symbolic ratio realization is numerically wrong");

    PitchSystem contradiction;
    contradiction.addVariable(tonic);
    contradiction.addEquation({
        {{tonic, Rational(1)}},
        PitchExpression::cents(Rational(0)),
        "first anchor",
    });
    contradiction.addEquation({
        {{tonic, Rational(1)}},
        PitchExpression::cents(Rational(1)),
        "contradictory anchor",
    });
    require(
        !contradiction.solve().has_value(),
        "contradictory hard pitch constraints were averaged");
}

