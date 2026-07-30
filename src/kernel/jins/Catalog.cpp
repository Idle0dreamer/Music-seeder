#include "Internal.hpp"

namespace mq::kernel::jins {

std::expected<void, std::string> Catalog::add(
    Descriptor descriptor) {
    const auto valid = detail::validate(descriptor);
    if (!valid) {
        return std::unexpected(valid.error());
    }
    if (descriptors_.contains(descriptor.identity)) {
        return std::unexpected(
            "jins descriptor identity is duplicated: " +
            descriptor.identity.str());
    }
    descriptors_.emplace(descriptor.identity, std::move(descriptor));
    return {};
}

const Descriptor* Catalog::find(
    const Identity& identity) const noexcept {
    const auto found = descriptors_.find(identity);
    return found == descriptors_.end() ? nullptr : &found->second;
}

std::size_t Catalog::size() const noexcept {
    return descriptors_.size();
}

} // namespace mq::kernel::jins
