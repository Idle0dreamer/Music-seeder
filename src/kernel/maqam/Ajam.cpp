#include "mq/kernel/maqam/Ajam.hpp"

#include "mq/kernel/maqam/Family.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_ajam() {
    const std::string source =
        "MaqamWorld:maqam-ajam;MaqamWorld:jins-ajam;"
        "MaqamWorld:jins-upper-ajam;MaqamWorld:jins-nahawand;"
        "scope:canonical-arabic-repertoire-v1";
    return family::make(family::Spec{
        "ajam",
        "ajam",
        pitch::Expression::ratio(3, 2),
        pitch::Expression::ratio(2, 1),
        performance::Timing{
            {Rational(3, 2), Rational(3, 4), performance::Articulation::Neutral},
            {Rational(1), Rational(1), performance::Articulation::Connected},
            {Rational(3, 4), Rational(1), performance::Articulation::Connected},
            {Rational(1, 2), Rational(7, 8), performance::Articulation::Detached},
            source + ";execution:ajam-v1",
        },
        source,
        {
            {
                "upper-ajam",
                pitch::Expression::ratio(3, 2),
                "MaqamWorld:maqam-ajam;MaqamWorld:jins-upper-ajam",
            },
            {
                "nahawand",
                pitch::Expression::ratio(3, 2),
                "MaqamWorld:maqam-ajam;MaqamWorld:jins-nahawand",
            },
        },
    });
}

} // namespace mq::kernel::maqam
