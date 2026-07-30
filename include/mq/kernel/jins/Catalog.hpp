#pragma once

#include "mq/kernel/jins/Descriptor.hpp"

#include <expected>
#include <map>
#include <string>

namespace mq::kernel::jins {

class Catalog {
public:
    [[nodiscard]] std::expected<void, std::string> add(
        Descriptor descriptor);

    [[nodiscard]] const Descriptor* find(
        const Identity& identity) const noexcept;

    [[nodiscard]] std::size_t size() const noexcept;

private:
    std::map<Identity, Descriptor> descriptors_;
};

} // namespace mq::kernel::jins
