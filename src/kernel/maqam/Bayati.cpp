#include "mq/kernel/maqam/Bayati.hpp"

#include "bayati/Internal.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_bayati() {
    const auto key = bayati::detail::keys();
    auto profile = bayati::detail::profile(key);
    if (!profile) {
        return std::unexpected(profile.error());
    }
    auto ajnas = bayati::detail::ajnas(key);
    if (!ajnas) {
        return std::unexpected(ajnas.error());
    }
    auto graph = bayati::detail::paths(key);
    if (!graph) {
        return std::unexpected(graph.error());
    }
    auto sayr = bayati::detail::sayr(key);
    if (!sayr) {
        return std::unexpected(sayr.error());
    }
    auto generation = bayati::detail::generation(key);
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
