#include "Internal.hpp"

namespace mq::kernel::pitch::field::assemble::detail {

std::expected<void, Error> check(
    const Need& need,
    const std::set<Identity>& rules,
    std::set<Identity>& all) {
    const auto added = add(all, need.identity);
    if (!added) {
        return std::unexpected(added.error());
    }
    if (need.any.empty()) {
        return std::unexpected(input(
            "field coverage set is empty",
            need.identity));
    }

    std::set<Identity> alternatives;
    for (const auto& alternative : need.any) {
        if (!complete(alternative) ||
            !alternatives.insert(alternative).second ||
            !rules.contains(alternative)) {
            return std::unexpected(input(
                "field coverage names an invalid rule",
                need.identity));
        }
    }
    return {};
}

} // namespace mq::kernel::pitch::field::assemble::detail
