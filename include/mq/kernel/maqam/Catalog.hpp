#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/maqam/Bayati.hpp"
#include "mq/kernel/maqam/Rast.hpp"
#include "mq/kernel/sort/FamilyId.hpp"
#include "mq/kernel/sort/MaqamId.hpp"

#include <expected>
#include <string>
#include <vector>

namespace mq::kernel::maqam {

enum class Implementation {
    Complete,
    Incomplete,
};

using Builder = std::expected<Scaffold, std::string> (*)();

struct Entry {
    sort::MaqamId id;
    sort::FamilyId family;
    std::string name;
    Implementation implementation;
    std::string provenance;
    Builder builder{};
};

class Catalog {
public:
    [[nodiscard]] static Catalog declared();

    [[nodiscard]] const std::vector<Entry>& entries() const noexcept;
    [[nodiscard]] const Entry* find(const sort::MaqamId& id) const noexcept;
    [[nodiscard]] std::expected<Scaffold, std::string> build(
        const sort::MaqamId& id) const;

private:
    std::vector<Entry> entries_;
};

} // namespace mq::kernel::maqam
