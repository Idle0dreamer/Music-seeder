#include "mq/kernel/maqam/collection/Build.hpp"

#include "mq/kernel/maqam/Family.hpp"
#include "mq/kernel/maqam/Bayati.hpp"
#include "mq/kernel/maqam/collection/Loader.hpp"

#include <algorithm>
#include <array>

namespace mq::kernel::maqam::collection {
namespace {

using Builder = std::expected<Scaffold, std::string> (*)();

struct Legacy {
    std::string_view name;
    Builder builder;
};

constexpr std::array legacy{
    Legacy{"bayati", ::mq::kernel::maqam::make_bayati},
};

const Legacy* find_legacy(std::string_view package) noexcept {
    const auto found = std::ranges::find(legacy, package, &Legacy::name);
    return found == legacy.end() ? nullptr : &*found;
}

} // namespace

std::expected<Scaffold, std::string> build(std::string_view package) {
    const auto set = load_default();
    if (!set) {
        return std::unexpected(set.error());
    }
    const auto record = std::ranges::find(
        set->records, package, &Record::name);
    if (record == set->records.end()) {
        return std::unexpected(
            "maqam collection has no package: " + std::string(package));
    }
    if (record->specification) {
        return family::make(*record->specification);
    }
    if (record->kind != "legacy") {
        return std::unexpected(
            "maqam collection record has no executable specification: " +
            std::string(package));
    }
    const auto* legacyBuilder = find_legacy(package);
    if (legacyBuilder == nullptr) {
        return std::unexpected(
            "maqam collection legacy record has no registered executor: " +
            std::string(package));
    }
    return legacyBuilder->builder();
}

} // namespace mq::kernel::maqam::collection
