#include "Internal.hpp"

namespace mq::kernel::maqam::rast::detail {

std::expected<path::Graph, std::string> paths(const Key& key) {
    path::Graph result;
    for (const auto& rule : {
             path::Rule{
                 key.pathUpperRast,
                 key.centerRoot,
                 key.centerUpper,
                 tonicization::Level::Internal,
                 {},
                 "MaqamWorld:maqam-rast;MaqamWorld:jins-upper-rast",
             },
             path::Rule{
                 key.pathNahawand,
                 key.centerRoot,
                 key.centerUpper,
                 tonicization::Level::Internal,
                 {},
                 "MaqamWorld:maqam-rast;MaqamWorld:maqam-nahawand",
             },
         }) {
        auto added = result.add(rule);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    return result;
}

} // namespace mq::kernel::maqam::rast::detail
