#include "mq/kernel/maqam/Ajam.hpp"

#include "mq/kernel/maqam/collection/Build.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_ajam() {
    return collection::build("ajam");
}

} // namespace mq::kernel::maqam
