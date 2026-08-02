#include "Internal.hpp"

namespace mq::kernel::maqam::bayati::detail {

performance::Timing timing() {
    return performance::Timing{
        {
            Rational(3, 2),
            Rational(3, 4),
            performance::Articulation::Neutral,
        },
        {
            Rational(1),
            Rational(1),
            performance::Articulation::Connected,
        },
        {
            Rational(3, 4),
            Rational(1),
            performance::Articulation::Connected,
        },
        {
            Rational(1, 2),
            Rational(7, 8),
            performance::Articulation::Detached,
        },
        "engineering:bayati-provisional-execution-v1;"
        "not-performance-authenticity-evidence",
    };
}

} // namespace mq::kernel::maqam::bayati::detail
