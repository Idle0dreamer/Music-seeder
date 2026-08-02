#pragma once

#include "mq/kernel/performance/TimedEvent.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace mq::synthesis::santur {

struct Config {
    std::uint32_t sample_rate{48'000};
    double tonic_hz{146.8323839587};
    double seconds_per_unit{};
    std::uint64_t seed{};
};

struct PreparedEvent {
    struct PitchPoint {
        double position{};
        double cents{};
    };

    double onset_seconds{};
    double duration_seconds{};
    double intensity{};
    double release_seconds{};
    double center_cents{};
    double strike_position{};
    double start_phase{};
    double decay{};
    double audible_until_seconds{};
    std::array<double, 36> mode_amplitudes{};
    std::array<double, 36> mode_multipliers{};
    std::vector<PitchPoint> contour;
    bool approach{};
    bool oscillation{};
    double ornament_onset{};
    double ornament_duration{};
    double ornament_extent_cents{};
    double ornament_cycles{};
};

class Model {
public:
    explicit Model(Config config) : config_(config) {}

    [[nodiscard]] PreparedEvent prepare(
        const ::mq::kernel::performance::TimedEvent& event) const;

    [[nodiscard]] double sample(
        const PreparedEvent& event,
        double seconds) const;

    [[nodiscard]] double sample(
        const ::mq::kernel::performance::TimedEvent& event,
        double seconds) const {
        return sample(prepare(event), seconds);
    }

private:
    Config config_;
};

} // namespace mq::synthesis::santur
