#pragma once

#include "mq/kernel/maqam/Family.hpp"

#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace mq::kernel::maqam::collection {

struct Record {
    std::string name;
    std::string family;
    bool complete{};
    std::string provenance;
    std::string kind;
    std::optional<family::Spec> specification;
};

struct Set {
    std::string identity;
    std::vector<Record> records;
};

[[nodiscard]] std::expected<Set, std::string> load(
    const std::filesystem::path& path);

[[nodiscard]] std::expected<Set, std::string> load_default();

[[nodiscard]] std::expected<family::Spec, std::string> find_spec(
    const Set& set,
    std::string_view name);

[[nodiscard]] std::expected<family::Spec, std::string> find_spec_default(
    std::string_view name);

} // namespace mq::kernel::maqam::collection
