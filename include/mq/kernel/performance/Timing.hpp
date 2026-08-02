#pragma once

#include "mq/kernel/Rational.hpp"
#include "mq/kernel/motion/Direction.hpp"
#include "mq/kernel/performance/Articulation.hpp"
#include "mq/kernel/performance/Ornament.hpp"

#include <string>
#include <cstddef>
#include <cstdint>

namespace mq::kernel::performance {

struct TimingIntent {
    Rational duration;
    Rational intensity;
    Articulation articulation;
    Rational release_duration;
    Articulation release_articulation;
    OrnamentTiming ornament_timing;

    [[nodiscard]] bool well_formed() const {
        bool known_articulation = false;
        bool known_release_articulation = false;
        switch (articulation) {
        case Articulation::Neutral:
        case Articulation::Connected:
        case Articulation::Detached:
            known_articulation = true;
            break;
        }
        switch (release_articulation) {
        case Articulation::Neutral:
        case Articulation::Connected:
        case Articulation::Detached:
            known_release_articulation = true;
            break;
        }
        return duration > Rational(0) && intensity >= Rational(0) &&
               release_duration >= Rational(0) && known_articulation &&
               known_release_articulation && ornament_timing.well_formed();
    }

    bool operator==(const TimingIntent&) const = default;
};

struct Context {
    std::size_t ordinal{};
    bool phrase_start{};
    bool phrase_end{};
    bool cadence{};
    std::uint64_t seed{};
};

struct Timing {
    TimingIntent start;
    TimingIntent same;
    TimingIntent rise;
    TimingIntent fall;
    Rational phrase_start_rate;
    Rational phrase_body_rate;
    Rational phrase_end_rate;
    Rational duration_variation;
    Rational intensity_variation;
    Rational open_pause;
    Rational closed_pause;
    Rational seconds_per_unit;
    Rational tail_seconds;
    std::string provenance;

    [[nodiscard]] bool well_formed() const {
        return start.well_formed() && same.well_formed() &&
               rise.well_formed() && fall.well_formed() &&
               phrase_start_rate > Rational(0) &&
               phrase_body_rate > Rational(0) && phrase_end_rate > Rational(0) &&
               duration_variation >= Rational(0) &&
               duration_variation < Rational(1) &&
               intensity_variation >= Rational(0) &&
               intensity_variation < Rational(1) && open_pause >= Rational(0) &&
               closed_pause >= Rational(0) && seconds_per_unit > Rational(0) &&
               tail_seconds >= Rational(0) && !provenance.empty();
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

    [[nodiscard]] TimingIntent resolve(
        motion::Direction direction,
        const Context& context) const;

    [[nodiscard]] Rational pause(const Context& context) const noexcept {
        return context.cadence || context.phrase_end ? closed_pause
                                                       : open_pause;
    }

    bool operator==(const Timing&) const = default;
};

} // namespace mq::kernel::performance
