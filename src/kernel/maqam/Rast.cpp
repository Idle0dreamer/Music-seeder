#include "mq/kernel/maqam/Rast.hpp"

#include "mq/kernel/maqam/collection/Build.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_rast() {
    return collection::build("rast");
}

} // namespace mq::kernel::maqam
