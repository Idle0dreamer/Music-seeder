#pragma once

#include "mq/kernel/Rational.hpp"
#include "mq/kernel/motion/Direction.hpp"
#include "mq/kernel/performance/Articulation.hpp"

#include <string>

namespace mq::kernel::performance {

struct TimingIntent {
    Rational duration{1};
    Rational intensity{1};
    Articulation articulation{Articulation::Neutral};

    [[nodiscard]] bool well_formed() const {
        bool known_articulation = false;
        switch (articulation) {
        case Articulation::Neutral:
        case Articulation::Connected:
        case Articulation::Detached:
            known_articulation = true;
            break;
        }
        return duration > Rational(0) && intensity >= Rational(0) &&
               known_articulation;
    }

    bool operator==(const TimingIntent&) const = default;
};

struct Timing {
    TimingIntent start;
    TimingIntent same;
    TimingIntent rise;
    TimingIntent fall;
    std::string provenance{"neutral:unit-execution-v1"};

    [[nodiscard]] bool well_formed() const {
        return start.well_formed() && same.well_formed() &&
               rise.well_formed() && fall.well_formed() &&
               !provenance.empty();
    }

    [[nodiscard]] const TimingIntent& for_direction(
        motion::Direction direction) const noexcept {
        switch (direction) {
        case motion::Direction::Start:
            return start;
        case motion::Direction::Same:
            return same;
        case motion::Direction::Rise:
            return rise;
        case motion::Direction::Fall:
            return fall;
        }
        return start;
    }

    bool operator==(const Timing&) const = default;
};

} // namespace mq::kernel::performance
