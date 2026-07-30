#include "mq/kernel/Identity.hpp"

namespace mq::kernel {

std::string Identity::str() const {
    return nameSpace + ":" + localName + "@" + revision;
}

} // namespace mq::kernel

