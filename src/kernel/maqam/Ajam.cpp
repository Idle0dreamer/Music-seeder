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
