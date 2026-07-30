#include "Internal.hpp"

namespace mq::kernel::pitch::field::assemble::detail {

Identity identity(const Rule& rule) {
    return std::visit(
        [](const auto& effect) -> Identity {
            using Type = std::decay_t<decltype(effect)>;
            if constexpr (std::same_as<Type, Aim>) {
                return effect.target.identity;
            } else {
                return effect.identity;
            }
        },
        rule.effect);
}

} // namespace mq::kernel::pitch::field::assemble::detail
