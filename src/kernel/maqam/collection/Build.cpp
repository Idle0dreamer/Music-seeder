#include "mq/kernel/maqam/collection/Build.hpp"

#include "mq/kernel/maqam/Family.hpp"
#include "mq/kernel/maqam/collection/Loader.hpp"

#include <algorithm>

namespace mq::kernel::maqam::collection {

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
    return std::unexpected(
        "maqam collection record has no executable specification: " +
        std::string(package));
}

} // namespace mq::kernel::maqam::collection
