#include "mq/kernel/evidence/Kind.hpp"

namespace mq::kernel::evidence {

const char* name(Kind value) noexcept {
    switch (value) {
    case Kind::Recurrence:
        return "recurrence";
    case Kind::Dwell:
        return "dwell";
    case Kind::Emphasis:
        return "emphasis";
    case Kind::Cell:
        return "cell";
    case Kind::Baggage:
        return "baggage";
    case Kind::Cadence:
        return "cadence";
    case Kind::Register:
        return "register";
    }
    return "unknown";
}

} // namespace mq::kernel::evidence
