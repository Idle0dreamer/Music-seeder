#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

#include <variant>

namespace mq::kernel {

enum class EvidenceKind {
    Recurrence,
    Dwell,
    Emphasis,
    Cell,
    Baggage,
    Cadence,
    Register,
};

enum class TonicizationLevel {
    Color,
    Internal,
    Maqam,
};

struct Anchor {
    Identity center;
};

struct Enter {
    Identity jins;
};

struct Emphasize {
    Identity role;
    Rational amount{1};
};

struct Dwell {
    Identity role;
    Rational amount{1};
};

struct Emit {
    Identity cell;
};

struct Cadence {
    Identity family;
    Rational evidence{1};
};

struct Tonicize {
    Identity jins;
    TonicizationLevel level{TonicizationLevel::Color};
};

struct Modulate {
    Identity path;
    Identity targetCenter;
    TonicizationLevel level{TonicizationLevel::Internal};
};

struct Return {
    Identity center;
};

using Operation = std::variant<
    Anchor,
    Enter,
    Emphasize,
    Dwell,
    Emit,
    Cadence,
    Tonicize,
    Modulate,
    Return>;

[[nodiscard]] const char* name(EvidenceKind value) noexcept;
[[nodiscard]] const char* name(TonicizationLevel value) noexcept;
[[nodiscard]] const char* name(const Operation& value) noexcept;

} // namespace mq::kernel

