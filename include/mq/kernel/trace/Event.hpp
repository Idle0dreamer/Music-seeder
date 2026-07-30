#pragma once

#include <cstddef>
#include <string>

namespace mq::kernel::trace {

struct Event {
    std::size_t index{};
    std::string operation;
    std::string subject;

    bool operator==(const Event&) const = default;
};

} // namespace mq::kernel::trace
