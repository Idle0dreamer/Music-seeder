#include "mq/kernel/Operator.hpp"

namespace mq::kernel {

const char* name(EvidenceKind value) noexcept {
    switch (value) {
    case EvidenceKind::Recurrence:
        return "recurrence";
    case EvidenceKind::Dwell:
        return "dwell";
    case EvidenceKind::Emphasis:
        return "emphasis";
    case EvidenceKind::Cell:
        return "cell";
    case EvidenceKind::Baggage:
        return "baggage";
    case EvidenceKind::Cadence:
        return "cadence";
    case EvidenceKind::Register:
        return "register";
    }
    return "unknown";
}

const char* name(TonicizationLevel value) noexcept {
    switch (value) {
    case TonicizationLevel::Color:
        return "color";
    case TonicizationLevel::Internal:
        return "internal";
    case TonicizationLevel::Maqam:
        return "maqam";
    }
    return "unknown";
}

const char* name(const Operation& value) noexcept {
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
            } else {
                return "Return";
            }
        },
        value);
}

} // namespace mq::kernel

