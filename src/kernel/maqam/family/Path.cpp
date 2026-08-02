#include "Internal.hpp"

namespace mq::kernel::maqam::family::detail {

std::expected<path::Graph, std::string> paths(const Key& key) {
    path::Graph result;
    for (const auto& branch : key.branches) {
        auto added = result.add(path::Rule{
            branch.path,
            key.centerRoot,
            key.centerUpper,
            tonicization::Level::Internal,
            {},
            branch.source,
        });
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    return result;
}

} // namespace mq::kernel::maqam::family::detail
