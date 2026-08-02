#include "mq/kernel/maqam/Catalog.hpp"
#include "mq/kernel/maqam/collection/Build.hpp"

#include <algorithm>

namespace mq::kernel::maqam {
namespace {

Identity id(const std::string& name) {
    return {"maqam.catalog", name, "1"};
}

Entry entry(const collection::Record& record) {
    return {
        sort::MaqamId{id("maqam." + record.name)},
        sort::FamilyId{id("family." + record.family)},
        record.name,
        record.complete ? Implementation::Complete : Implementation::Incomplete,
        record.provenance,
        record.kind,
        record.specification,
    };
}

} // namespace

Catalog Catalog::declared() {
    Catalog result;
    const auto collection = collection::load_default();
    if (!collection) {
        result.load_error_ = collection.error();
        return result;
    }
    result.entries_.reserve(collection->records.size());
    for (const auto& record : collection->records) {
        result.entries_.push_back(entry(record));
    }
    return result;
}

const std::vector<Entry>& Catalog::entries() const noexcept {
    return entries_;
}

const Entry* Catalog::find(const sort::MaqamId& id) const noexcept {
    const auto found = std::ranges::find(entries_, id, &Entry::id);
    return found == entries_.end() ? nullptr : &*found;
}

const Entry* Catalog::find(std::string_view name) const noexcept {
    const auto found = std::ranges::find(entries_, name, &Entry::name);
    return found == entries_.end() ? nullptr : &*found;
}

std::expected<Scaffold, std::string> Catalog::build(
    const sort::MaqamId& id) const {
    if (!load_error_.empty()) {
        return std::unexpected(load_error_);
    }
    const auto* found = find(id);
    if (found == nullptr) {
        return std::unexpected("maqam is not declared in the catalog");
    }
    if (found->implementation != Implementation::Complete) {
        return std::unexpected(
            "maqam package is declared but not complete: " + found->name);
    }
    if (!found->specification) {
        return std::unexpected(
            "maqam package has no executable collection specification: " +
            found->name);
    }
    return family::make(*found->specification);
}

std::expected<Scaffold, std::string> Catalog::build(
    std::string_view name) const {
    const auto* found = find(name);
    if (found == nullptr) {
        if (!load_error_.empty()) {
            return std::unexpected(load_error_);
        }
        return std::unexpected("maqam is not declared in the catalog");
    }
    return build(found->id);
}

std::expected<Scaffold, std::string> Catalog::build_executable(
    std::string_view name) const {
    if (!load_error_.empty()) {
        return std::unexpected(load_error_);
    }
    const auto* found = find(name);
    if (found == nullptr) {
        return std::unexpected("maqam is not declared in the catalog");
    }
    return collection::build(name);
}

} // namespace mq::kernel::maqam
