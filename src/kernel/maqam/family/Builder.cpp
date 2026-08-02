#include "Internal.hpp"

namespace mq::kernel::maqam::family {

std::expected<Scaffold, std::string> make(const Spec& spec) {
    const auto keys = detail::key(spec);
    auto ajnas = detail::ajnas(keys);
    if (!ajnas) {
        return std::unexpected(ajnas.error());
    }
    auto graph = detail::paths(keys);
    if (!graph) {
        return std::unexpected(graph.error());
    }
    auto sayr = detail::sayr(keys);
    if (!sayr) {
        return std::unexpected(sayr.error());
    }
    auto profile = detail::profile(keys);
    if (!profile) {
        return std::unexpected(profile.error());
    }
    auto generation = detail::generation(keys);
    if (!generation) {
        return std::unexpected(generation.error());
    }
    return Scaffold{
        std::move(*ajnas),
        std::move(*graph),
        std::move(*sayr),
        std::move(*profile),
        std::move(*generation),
    };
}

} // namespace mq::kernel::maqam::family
