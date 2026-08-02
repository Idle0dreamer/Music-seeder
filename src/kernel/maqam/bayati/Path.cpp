#include "Internal.hpp"

namespace mq::kernel::maqam::bayati::detail {

std::expected<path::Graph, std::string> paths(const Key& key) {
    path::Graph result;
    for (const auto& rule : {
             path::Rule{
                 key.pathNahawand,
                 key.centerRoot,
                 key.centerUpper,
                 tonicization::Level::Internal,
                 {},
                 "AbuShumays2013:pp.22-24;MaqamWorld:maqam-bayati",
             },
             path::Rule{
                 key.pathRast,
                 key.centerRoot,
                 key.centerUpper,
                 tonicization::Level::Internal,
                 {},
                 "AbuShumays2013:pp.3-4,22-24;MaqamWorld:maqam-bayati",
             },
             path::Rule{
                 key.pathHijaz,
                 key.centerRoot,
                 key.centerUpper,
                 tonicization::Level::Internal,
                 {},
                 "AbuShumays2013:pp.22-23",
             },
         }) {
        auto added = result.add(rule);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    return result;
}

} // namespace mq::kernel::maqam::bayati::detail
