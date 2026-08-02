#include "mq/kernel/maqam/Nikriz.hpp"

#include "mq/kernel/maqam/Family.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_nikriz() {
    const std::string source =
        "MaqamWorld:maqam-nikriz;MaqamWorld:jins-nikriz;"
        "MaqamWorld:jins-nahawand;scope:canonical-arabic-repertoire-v1";
    return family::make(family::Spec{
        "nikriz",
        "nikriz",
        pitch::Expression::ratio(3, 2),
        pitch::Expression::ratio(2, 1),
        source,
        {
            {
                "nahawand",
                pitch::Expression::ratio(3, 2),
                "MaqamWorld:maqam-nikriz;MaqamWorld:jins-nahawand",
            },
        },
    });
}

} // namespace mq::kernel::maqam
