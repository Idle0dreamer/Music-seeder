#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/maqam/Package.hpp"
#include "mq/kernel/maqam/Family.hpp"
#include "mq/kernel/maqam/collection/Loader.hpp"
#include "mq/kernel/sort/FamilyId.hpp"
#include "mq/kernel/sort/MaqamId.hpp"

#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace mq::kernel::maqam {

enum class Implementation {
    Complete,
    Incomplete,
};

struct Entry {
    sort::MaqamId id;
    sort::FamilyId family;
    std::string name;
    Implementation implementation;
    std::string provenance;
    std::string kind;
    std::optional<family::Spec> specification;
};

class Catalog {
public:
    [[nodiscard]] static Catalog declared();

    [[nodiscard]] const std::vector<Entry>& entries() const noexcept;
    [[nodiscard]] const Entry* find(const sort::MaqamId& id) const noexcept;
    [[nodiscard]] const Entry* find(std::string_view name) const noexcept;
    [[nodiscard]] std::expected<Scaffold, std::string> build(
        const sort::MaqamId& id) const;
    [[nodiscard]] std::expected<Scaffold, std::string> build(
        std::string_view name) const;
    [[nodiscard]] std::expected<Scaffold, std::string> build_executable(
        std::string_view name) const;

private:
    std::vector<Entry> entries_;
    std::string load_error_;
};

} // namespace mq::kernel::maqam
