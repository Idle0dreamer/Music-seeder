#pragma once

#include "mq/kernel/performance/TimedEvent.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace mq::synthesis::pitch {

[[nodiscard]] inline double contour_cents(
    const ::mq::kernel::performance::TimedEvent& event,
    double position) {
    if (!event.contour || event.contour->points.empty()) {
        return 0.0;
    }
    const auto& points = event.contour->points;
    const double clamped = std::clamp(position, 0.0, 1.0);
    if (clamped <= points.front().position.decimal()) {
        return points.front().offset.cents();
    }
    for (std::size_t index = 1; index < points.size(); ++index) {
        const double right = points[index].position.decimal();
        if (clamped <= right) {
            const double left = points[index - 1].position.decimal();
            const double span = right - left;
            const double amount = span > 0.0 ? (clamped - left) / span : 0.0;
            const double low = points[index - 1].offset.cents();
            const double high = points[index].offset.cents();
            return low + amount * (high - low);
        }
    }
    return points.back().offset.cents();
}

[[nodiscard]] inline double frequency_hz(
    const ::mq::kernel::performance::TimedEvent& event,
    double tonic_hz,
    double position) {
    double ornament = 0.0;
    if (event.ornament && event.ornament->kind ==
                              ::mq::kernel::performance::OrnamentKind::Oscillation) {
        const auto& timing = event.ornament->timing;
        const double start = timing.onset.decimal();
        const double end = start + timing.duration.decimal();
        const double clamped = std::clamp(position, 0.0, 1.0);
        if (clamped >= start && clamped <= end && end > start) {
            const double local = (clamped - start) / (end - start);
            ornament = event.ornament->extent.cents() * std::sin(
                2.0 * std::numbers::pi_v<double> *
                event.ornament->cycles.decimal() * local);
        }
    }
    double contour_position = std::clamp(position, 0.0, 1.0);
    if (event.ornament && event.ornament->kind ==
                              ::mq::kernel::performance::OrnamentKind::Approach) {
        const auto& timing = event.ornament->timing;
        const double start = timing.onset.decimal();
        const double end = start + timing.duration.decimal();
        if (contour_position <= start) {
            contour_position = 0.0;
        } else if (contour_position >= end) {
            contour_position = 1.0;
        } else if (end > start) {
            contour_position = (contour_position - start) / (end - start);
        }
    }
    return tonic_hz * std::exp2(
        (event.target.center.cents() + contour_cents(event, contour_position) +
         ornament) /
        1200.0);
}

} // namespace mq::synthesis::pitch
