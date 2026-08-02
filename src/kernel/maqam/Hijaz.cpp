#include "mq/kernel/maqam/Hijaz.hpp"

#include "mq/kernel/maqam/collection/Build.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_hijaz() {
    return collection::build("hijaz");
}

} // namespace mq::kernel::maqam
