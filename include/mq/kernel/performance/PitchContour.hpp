#pragma once

#include "mq/kernel/Rational.hpp"
#include "mq/kernel/pitch/Expression.hpp"

#include <vector>

namespace mq::kernel::performance {

// Offsets are exact symbolic pitch expressions relative to the event center.
// The adapter may interpolate their numerical values only at its boundary.
struct PitchPoint {
    Rational position{};
    pitch::Expression offset{};

    bool operator==(const PitchPoint&) const = default;
};

struct PitchContour {
    std::vector<PitchPoint> points;

    [[nodiscard]] bool well_formed() const {
        if (points.empty()) {
            return true;
        }
        if (points.size() < 2 || points.front().position != Rational(0) ||
            points.back().position != Rational(1)) {
            return false;
        }
        Rational previous{};
        bool first = true;
        for (const auto& point : points) {
            if (point.position < Rational(0) || point.position > Rational(1) ||
                (!first && point.position <= previous)) {
                return false;
            }
            previous = point.position;
            first = false;
        }
        return true;
    }

    bool operator==(const PitchContour&) const = default;
};

} // namespace mq::kernel::performance
