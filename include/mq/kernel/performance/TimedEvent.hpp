#pragma once

#include "mq/kernel/Rational.hpp"
#include "mq/kernel/performance/Articulation.hpp"
#include "mq/kernel/performance/Ornament.hpp"
#include "mq/kernel/performance/PitchContour.hpp"
#include "mq/kernel/performance/Strand.hpp"
#include "mq/kernel/performance/Target.hpp"

#include <optional>

namespace mq::kernel::performance {

struct TimedEvent {
    Target target;
    Rational onset;
    Rational duration;
    Rational intensity;
    Articulation articulation;
    sort::StrandId strand;
    std::optional<PitchContour> contour;
    std::optional<Ornament> ornament;

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
               (!contour || contour->well_formed()) &&
               (!ornament || ornament->well_formed()) &&
               !strand.identity.domain.empty() &&
               !strand.identity.name.empty() &&
               !strand.identity.revision.empty();
    }

    bool operator==(const TimedEvent&) const = default;
};

} // namespace mq::kernel::performance
