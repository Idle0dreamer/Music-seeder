#include "mq/kernel/maqam/Nikriz.hpp"

#include "mq/kernel/maqam/collection/Build.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_nikriz() {
    return collection::build("nikriz");
}

} // namespace mq::kernel::maqam
