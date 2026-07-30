#pragma once

#include <compare>
#include <string>

namespace mq::kernel {

struct Identity {
    std::string domain;
    std::string name;
    std::string revision{"1"};

    [[nodiscard]] std::string str() const;

    auto operator<=>(const Identity&) const = default;
};

} // namespace mq::kernel
