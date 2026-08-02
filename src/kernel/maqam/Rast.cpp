#include "mq/kernel/maqam/Rast.hpp"

#include "rast/Internal.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_rast() {
    const auto key = rast::detail::keys();
    auto profile = rast::detail::profile(key);
    if (!profile) {
        return std::unexpected(profile.error());
    }
    auto ajnas = rast::detail::ajnas(key);
    if (!ajnas) {
        return std::unexpected(ajnas.error());
    }
    auto graph = rast::detail::paths(key);
    if (!graph) {
        return std::unexpected(graph.error());
    }
    auto sayr = rast::detail::sayr(key);
    if (!sayr) {
        return std::unexpected(sayr.error());
    }
    auto generation = rast::detail::generation(key);
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

} // namespace mq::kernel::maqam
