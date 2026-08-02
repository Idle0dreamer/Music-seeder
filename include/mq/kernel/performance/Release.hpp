#pragma once

#include "mq/kernel/Rational.hpp"
#include "mq/kernel/performance/Articulation.hpp"

namespace mq::kernel::performance {

struct Release {
    Rational duration;
    Articulation articulation;

    [[nodiscard]] bool well_formed() const {
        bool known_articulation = false;
        switch (articulation) {
        case Articulation::Neutral:
        case Articulation::Connected:
        case Articulation::Detached:
            known_articulation = true;
            break;
        }
        return duration >= Rational(0) && known_articulation;
    }

    bool operator==(const Release&) const = default;
};

} // namespace mq::kernel::performance
