#include "Run.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/pitch/System.hpp"

#include <iostream>

namespace app::pitch {

int run() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    if (!made) {
        std::cerr << made.error() << '\n';
        return 1;
    }

    const Identity tonic{"demo.pitch", "tonic", "1"};
    const Identity ghammaz{"demo.pitch", "ghammaz", "1"};
    mq::kernel::pitch::System field;
    field.declare(tonic);
    field.declare(ghammaz);
    field.equate({
        {"demo.constraint", "anchor", "1"},
        {{tonic, Rational(1)}},
        mq::kernel::pitch::Expression::cents(Rational(0)),
        "demo anchor",
    });
    field.equate({
        {"demo.constraint", "ghammaz", "1"},
        {{ghammaz, Rational(1)}, {tonic, Rational(-1)}},
        mq::kernel::pitch::Expression::ratio(4, 3),
        "demo ratio constraint",
    });
    const auto solution = field.solve();
    if (!solution) {
        std::cerr << solution.error() << '\n';
        return 1;
    }

    std::cout
        << "kernel: non-ML typed operator and constraint system\n"
        << "shared profile rules: "
        << made->profile.shared.rules().size() << '\n'
        << "symbolic ghammaz: "
        << solution->values.at(ghammaz).str() << '\n'
        << "realized cents: "
        << solution->values.at(ghammaz).cents() << '\n';
    return 0;
}

} // namespace app::pitch
