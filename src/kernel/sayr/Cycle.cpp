#include "Internal.hpp"

#include <functional>

namespace mq::kernel::sayr::detail {

std::expected<void, std::string> acyclic(
    const std::map<Identity, Obligation>& obligations) {
    std::map<Identity, int> marks;
    std::function<bool(const Identity&)> visit =
        [&](const Identity& identity) {
            auto& mark = marks[identity];
            if (mark == 1) {
                return false;
            }
            if (mark == 2) {
                return true;
            }
            mark = 1;
            for (const auto& predecessor :
                 obligations.at(identity).after) {
                if (!visit(predecessor)) {
                    return false;
                }
            }
            mark = 2;
            return true;
        };
    for (const auto& [identity, obligation] : obligations) {
        static_cast<void>(obligation);
        if (!visit(identity)) {
            return std::unexpected("sayr obligation graph is cyclic");
        }
    }
    return {};
}

} // namespace mq::kernel::sayr::detail
