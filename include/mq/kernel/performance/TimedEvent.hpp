#pragma once

#include "mq/kernel/Rational.hpp"
#include "mq/kernel/performance/Articulation.hpp"
#include "mq/kernel/performance/Strand.hpp"
#include "mq/kernel/performance/Target.hpp"

namespace mq::kernel::performance {

struct TimedEvent {
    Target target;
    Rational onset{};
    Rational duration{1};
    Rational intensity{1};
    Articulation articulation{Articulation::Neutral};
    sort::StrandId strand;

    [[nodiscard]] bool well_formed() const {
        bool known_articulation = false;
        switch (articulation) {
        case Articulation::Neutral:
        case Articulation::Connected:
        case Articulation::Detached:
            known_articulation = true;
            break;
        }
        return onset >= Rational(0) &&
               duration > Rational(0) &&
               intensity >= Rational(0) &&
               known_articulation &&
               !strand.identity.domain.empty() &&
               !strand.identity.name.empty() &&
               !strand.identity.revision.empty();
    }

    bool operator==(const TimedEvent&) const = default;
};

} // namespace mq::kernel::performance
