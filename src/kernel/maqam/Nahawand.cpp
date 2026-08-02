#include "mq/kernel/maqam/Nahawand.hpp"

#include "mq/kernel/maqam/Family.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_nahawand() {
    const std::string source =
        "MaqamWorld:maqam-nahawand;MaqamWorld:jins-nahawand;"
        "MaqamWorld:jins-hijaz;MaqamWorld:jins-kurd;"
        "scope:canonical-arabic-repertoire-v1";
    return family::make(family::Spec{
        "nahawand",
        "nahawand",
        pitch::Expression::ratio(3, 2),
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
            source + ";execution:nahawand-v1",
        },
        source,
        {
            {
                "hijaz",
                pitch::Expression::ratio(5, 3),
                "MaqamWorld:maqam-nahawand;MaqamWorld:jins-hijaz",
            },
            {
                "kurd",
                pitch::Expression::ratio(3, 2),
                "MaqamWorld:maqam-nahawand;MaqamWorld:jins-kurd",
            },
        },
    });
}

} // namespace mq::kernel::maqam
