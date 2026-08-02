#include "Internal.hpp"

namespace mq::kernel::maqam::family::detail {

performance::Timing timing(const Key& key) {
    return key.timing;
}

} // namespace mq::kernel::maqam::family::detail
