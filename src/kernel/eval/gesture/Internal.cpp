#include "Internal.hpp"

namespace mq::kernel::eval::gesture {

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

Violation reject(
    std::size_t index,
    const char* operation,
    std::string rule,
    std::string message) {
    return {
        index,
        operation,
        std::move(rule),
        std::move(message),
    };
}

} // namespace mq::kernel::eval::gesture
