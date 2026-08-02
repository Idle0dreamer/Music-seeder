#include "mq/kernel/maqam/Kurd.hpp"

#include "mq/kernel/maqam/Family.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_kurd() {
    const std::string source =
        "MaqamWorld:maqam-kurd;MaqamWorld:jins-kurd;"
        "MaqamWorld:jins-nahawand;scope:canonical-arabic-repertoire-v1";
    return family::make(family::Spec{
        "kurd",
        "kurd",
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
            source + ";execution:kurd-v1",
        },
        source,
        {
            {
                "nahawand",
                pitch::Expression::ratio(4, 3),
                "MaqamWorld:maqam-kurd;MaqamWorld:jins-nahawand",
            },
        },
    });
}

} // namespace mq::kernel::maqam
