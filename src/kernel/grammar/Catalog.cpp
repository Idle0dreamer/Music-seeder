#include "mq/kernel/grammar/Catalog.hpp"

namespace mq::kernel::grammar {

std::expected<void, std::string> Catalog::add(
    Identity identity,
    Term term) {
    if (productions_.contains(identity)) {
        return std::unexpected(
            "grammar production identity is duplicated: " + identity.str());
    }
    productions_.emplace(std::move(identity), std::move(term));
    return {};
}

const Term* Catalog::find(const Identity& identity) const noexcept {
    const auto found = productions_.find(identity);
    return found == productions_.end() ? nullptr : &found->second;
}

std::size_t Catalog::size() const noexcept {
    return productions_.size();
}

} // namespace mq::kernel::grammar
