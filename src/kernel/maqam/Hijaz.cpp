#include "mq/kernel/maqam/Hijaz.hpp"

#include "mq/kernel/maqam/Family.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_hijaz() {
    const std::string source =
        "MaqamWorld:maqam-hijaz;MaqamWorld:jins-hijaz;"
        "MaqamWorld:jins-nahawand;MaqamWorld:jins-rast;"
        "scope:canonical-arabic-repertoire-v1";
    return family::make(family::Spec{
        "hijaz",
        "hijaz",
        pitch::Expression::ratio(4, 3),
        pitch::Expression::ratio(2, 1),
        performance::Timing{
            {
                Rational(3, 2),
                Rational(3, 4),
                performance::Articulation::Neutral,
            },
            {
                Rational(1),
                Rational(1),
                performance::Articulation::Connected,
            },
            {
                Rational(3, 4),
                Rational(1),
                performance::Articulation::Connected,
            },
            {
                Rational(1, 2),
                Rational(7, 8),
                performance::Articulation::Detached,
            },
            source + ";execution:hijaz-v1",
        },
        source,
        {
            {
                "nahawand",
                pitch::Expression::ratio(4, 3),
                "MaqamWorld:maqam-hijaz;MaqamWorld:jins-nahawand",
            },
            {
                "rast",
                pitch::Expression::ratio(4, 3),
                "MaqamWorld:maqam-hijaz;MaqamWorld:jins-rast",
            },
        },
    });
}

} // namespace mq::kernel::maqam
