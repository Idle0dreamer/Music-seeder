#include "mq/kernel/maqam/Sikah.hpp"

#include "mq/kernel/maqam/collection/Build.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_sikah() {
    return collection::build("sikah");
}

} // namespace mq::kernel::maqam
