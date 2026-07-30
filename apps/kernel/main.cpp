#include "mq/kernel/Evaluator.hpp"
#include "mq/kernel/Fixture.hpp"
#include "mq/kernel/Pitch.hpp"

#include <iostream>

int main() {
    using namespace mq::kernel;

    const auto fixture = makeFixture();
    if (!fixture) {
        std::cerr << fixture.error() << '\n';
        return 1;
    }

    const Identity tonic{"demo.pitch", "tonic", "1"};
    const Identity ghammaz{"demo.pitch", "ghammaz", "1"};
    PitchSystem field;
    field.addVariable(tonic);
    field.addVariable(ghammaz);
    field.addEquation({
        {{tonic, Rational(1)}},
        PitchExpression::cents(Rational(0)),
        "demo anchor",
    });
    field.addEquation({
        {{ghammaz, Rational(1)}, {tonic, Rational(-1)}},
        PitchExpression::ratio(4, 3),
        "demo ratio constraint",
    });
    const auto solution = field.solve();
    if (!solution) {
        std::cerr << solution.error() << '\n';
        return 1;
    }

    std::cout
        << "kernel: non-ML typed operator and constraint system\n"
        << "shared profile rules: " << fixture->shared.rules().size() << '\n'
        << "regional A emphasis threshold: "
        << fixture->regionalA
               .parameter("threshold.internal.emphasis")
               ->str()
        << '\n'
        << "regional B permits demo path: "
        << (fixture->regionalB.allows("allow.modulate", fixture->path)
                ? "yes"
                : "no")
        << '\n'
        << "symbolic ghammaz: " << solution->values.at(ghammaz).str() << '\n'
        << "realized cents: " << solution->values.at(ghammaz).toCents()
        << '\n';
    return 0;
}

