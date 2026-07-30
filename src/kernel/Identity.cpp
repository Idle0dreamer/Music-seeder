#include "mq/kernel/Identity.hpp"

namespace mq::kernel {

std::string Identity::str() const {
    return domain + ":" + name + "@" + revision;
}

} // namespace mq::kernel
