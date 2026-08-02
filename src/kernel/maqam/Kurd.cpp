#include "mq/kernel/maqam/Kurd.hpp"

#include "mq/kernel/maqam/collection/Build.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_kurd() {
    return collection::build("kurd");
}

} // namespace mq::kernel::maqam
