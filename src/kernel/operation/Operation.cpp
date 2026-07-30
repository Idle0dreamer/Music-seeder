#include "mq/kernel/operation/Operation.hpp"

namespace mq::kernel::operation {

const char* name(const Any& value) noexcept {
    return std::visit(
        [](const auto& operation) -> const char* {
            using T = std::decay_t<decltype(operation)>;
            if constexpr (std::is_same_v<T, Anchor>) {
                return "Anchor";
            } else if constexpr (std::is_same_v<T, Enter>) {
                return "Enter";
            } else if constexpr (std::is_same_v<T, Emphasize>) {
                return "Emphasize";
            } else if constexpr (std::is_same_v<T, Dwell>) {
                return "Dwell";
            } else if constexpr (std::is_same_v<T, Emit>) {
                return "Emit";
            } else if constexpr (std::is_same_v<T, Cadence>) {
                return "Cadence";
            } else if constexpr (std::is_same_v<T, Tonicize>) {
                return "Tonicize";
            } else if constexpr (std::is_same_v<T, Modulate>) {
                return "Modulate";
            } else if constexpr (std::is_same_v<T, Place>) {
                return "Place";
            } else if constexpr (std::is_same_v<T, Begin>) {
                return "Begin";
            } else if constexpr (std::is_same_v<T, End>) {
                return "End";
            } else if constexpr (std::is_same_v<T, gesture::Begin>) {
                return "Gesture.Begin";
            } else if constexpr (std::is_same_v<T, gesture::End>) {
                return "Gesture.End";
            } else if constexpr (std::is_same_v<T, sayr::Fulfill>) {
                return "Sayr.Fulfill";
            } else {
                return "Return";
            }
        },
        value);
}

} // namespace mq::kernel::operation
