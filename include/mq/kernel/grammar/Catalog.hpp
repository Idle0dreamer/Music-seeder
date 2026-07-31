#pragma once

#include "mq/kernel/grammar/Term.hpp"

#include <expected>
#include <map>
#include <string>

namespace mq::kernel::grammar {

class Catalog {
public:
    [[nodiscard]] std::expected<void, std::string> add(
        Identity identity,
        Term term);

    [[nodiscard]] const Term* find(const Identity& identity) const noexcept;

    [[nodiscard]] std::size_t size() const noexcept;

private:
    std::map<Identity, Term> productions_;
};

} // namespace mq::kernel::grammar
