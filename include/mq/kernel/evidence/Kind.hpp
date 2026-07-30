#pragma once

namespace mq::kernel::evidence {

enum class Kind {
    Recurrence,
    Dwell,
    Emphasis,
    Cell,
    Baggage,
    Cadence,
    Register,
};

[[nodiscard]] const char* name(Kind value) noexcept;

} // namespace mq::kernel::evidence
