#include "mq/kernel/maqam/Sikah.hpp"

#include "mq/kernel/maqam/Family.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_sikah() {
    const std::string source =
        "MaqamWorld:maqam-sikah;MaqamWorld:jins-sikah;"
        "MaqamWorld:jins-upper-rast;MaqamWorld:jins-rast;"
        "scope:canonical-arabic-repertoire-v1;route:ordered-stations";
    return family::make(family::Spec{
        "sikah",
        "sikah",
        pitch::Expression::ratio(4, 3),
        pitch::Expression::ratio(3, 2),
        source,
        {
            {
                "upper-rast",
                pitch::Expression::ratio(3, 2),
                "MaqamWorld:maqam-sikah;MaqamWorld:jins-upper-rast",
                "root",
                "third",
                "ghammaz",
                "upper",
            },
            {
                "rast",
                pitch::Expression::ratio(2, 1),
                "MaqamWorld:maqam-sikah;MaqamWorld:jins-rast",
                "third",
                "sixth",
                "upper",
                "extension",
                motion::Direction::Rise,
            },
        },
        true,
    });
}

} // namespace mq::kernel::maqam
