#include "mq/kernel/maqam/Nahawand.hpp"

#include "mq/kernel/maqam/collection/Build.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_nahawand() {
    return collection::build("nahawand");
}

} // namespace mq::kernel::maqam
