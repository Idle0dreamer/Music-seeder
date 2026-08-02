#include "mq/synthesis/santur/Model.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <numbers>
#include <string>

namespace mq::synthesis::santur {
namespace {

constexpr double pi = std::numbers::pi_v<double>;
constexpr double inverse_two_pi = 1.0 / (2.0 * pi);

const std::array<double, 16'385>& sine_table() {
    static const auto table = [] {
        std::array<double, 16'385> result{};
        for (std::size_t index = 0; index < 16'384; ++index) {
            result[index] = std::sin(
                2.0 * pi * static_cast<double>(index) / 16'384.0);
        }
        result.back() = result.front();
        return result;
    }();
    return table;
}

double sine_lookup(double radians) {
    constexpr double table_size = 16'384.0;
    const double cycles = radians * inverse_two_pi;
    const double wrapped = cycles - std::floor(cycles);
    const double scaled = wrapped * table_size;
    const auto index = static_cast<std::size_t>(scaled);
    const double fraction = scaled - static_cast<double>(index);
    const auto& table = sine_table();
    return table[index] * (1.0 - fraction) +
           table[index + 1] * fraction;
}

std::uint64_t hash(std::uint64_t seed, const std::string& value) {
    std::uint64_t result = 1469598103934665603ULL ^ seed;
    for (const auto byte : value) {
        result ^= static_cast<std::uint8_t>(byte);
        result *= 1099511628211ULL;
    }
    return result;
}

double phase(const ::mq::kernel::performance::TimedEvent& event,
             std::uint64_t seed,
             std::uint64_t salt) {
    const auto value = hash(seed + salt, event.target.event.identity.str());
    return static_cast<double>(value % 10000U) / 10000.0 * 2.0 * pi;
}

double sustain_decay(
    ::mq::kernel::performance::Articulation articulation) {
    switch (articulation) {
    case ::mq::kernel::performance::Articulation::Connected:
        return 1.65;
    case ::mq::kernel::performance::Articulation::Detached:
        return 3.1;
    case ::mq::kernel::performance::Articulation::Neutral:
        return 2.25;
    }
    return 2.25;
}

double contour_cents(const PreparedEvent& event, double position) {
    if (event.contour.empty()) {
        return 0.0;
    }
    const double clamped = std::clamp(position, 0.0, 1.0);
    if (clamped <= event.contour.front().position) {
        return event.contour.front().cents;
    }
    for (std::size_t index = 1; index < event.contour.size(); ++index) {
        const auto& right = event.contour[index];
        if (clamped <= right.position) {
            const auto& left = event.contour[index - 1];
            const double span = right.position - left.position;
            const double amount = span > 0.0
                                      ? (clamped - left.position) / span
                                      : 0.0;
            return left.cents + amount * (right.cents - left.cents);
        }
    }
    return event.contour.back().cents;
}

double frequency_hz(const PreparedEvent& event,
                    double tonicHz,
                    double position) {
    const double clamped = std::clamp(position, 0.0, 1.0);
    double ornament = 0.0;
    if (event.oscillation && clamped >= event.ornament_onset &&
        clamped <= event.ornament_onset + event.ornament_duration &&
        event.ornament_duration > 0.0) {
        const double local = (clamped - event.ornament_onset) /
                             event.ornament_duration;
        ornament = event.ornament_extent_cents * sine_lookup(
            2.0 * pi * event.ornament_cycles * local);
    }

    double contourPosition = clamped;
    if (event.approach) {
        const double end = event.ornament_onset + event.ornament_duration;
        if (contourPosition <= event.ornament_onset) {
            contourPosition = 0.0;
        } else if (contourPosition >= end) {
            contourPosition = 1.0;
        } else if (end > event.ornament_onset) {
            contourPosition = (contourPosition - event.ornament_onset) /
                              (end - event.ornament_onset);
        }
    }
    return tonicHz * std::exp2(
        (event.center_cents + contour_cents(event, contourPosition) +
         ornament) /
        1200.0);
}

double string_mode(
    double baseHz,
    double local,
    double duration,
    double releaseDuration,
    double intensity,
    const double* modeMultipliers,
    const double* modeAmplitudes,
    double startPhase,
    double decay) {
    double result = 0.0;
    constexpr double modal_floor = 1.0e-5;
    const double modal_limit = -std::log(modal_floor);
    for (int partial = 1; partial <= 12; ++partial) {
        const double n = static_cast<double>(partial);
        const double modeHz = baseHz * modeMultipliers[partial - 1];
        const double amplitude = (0.84 / n) * modeAmplitudes[partial - 1];
        const double modeDecay = decay + 0.28 * n +
                                 0.78 * std::max(0.0, n - 3.0);
        const double envelope = modeDecay * local;
        if (envelope > modal_limit) {
            continue;
        }
        result += amplitude * std::exp(-envelope) *
                  sine_lookup(2.0 * pi * modeHz * local + startPhase * n);
    }

    const double contact = 0.0035 + 0.0025 / (1.0 + intensity);
    const double contactPhase = std::clamp(local / contact, 0.0, 1.0);
    const double hammer = local < contact
                              ? sine_lookup(pi * contactPhase)
                              : 1.0;
    const double release = local < duration
                               ? 1.0
                               : std::exp(
                                     -decay * (local - duration) /
                                     std::max(0.001, releaseDuration));
    return intensity * 0.24 * hammer * result * release;
}

double hammer_transient(
    double baseHz,
    double local,
    double intensity,
    double strikePosition,
    double startPhase) {
    if (local < 0.0) {
        return 0.0;
    }
    // A struck santur has a short, bright contact transient before the
    // coupled courses dominate. This is a physical attack approximation,
    // not a replacement for measured hammer and bridge calibration.
    const double contact = std::exp(-1450.0 * local);
    const double scrape = std::exp(-4100.0 * local);
    const double click = sine_lookup(
        2.0 * pi * baseHz * (7.0 + 3.0 * strikePosition) * local +
        startPhase * 1.7);
    const double edge = sine_lookup(
        2.0 * pi * baseHz * (15.0 + 8.0 * strikePosition) * local +
        startPhase * 2.3);
    return intensity * (0.16 * contact * click + 0.045 * scrape * edge);
}

double bridge_and_board(
    double baseHz,
    double local,
    double intensity,
    double startPhase) {
    const double bridge = 0.11 * std::exp(-4.2 * local) *
                          sine_lookup(2.0 * pi * baseHz * 1.003 * local +
                                      startPhase * 0.7);
    const double boardEnvelope = std::exp(-0.78 * local);
    const double board = boardEnvelope *
                         (0.06 * sine_lookup(2.0 * pi * baseHz * 0.51 * local +
                                             startPhase * 1.3) +
                          0.045 * sine_lookup(2.0 * pi * baseHz * 1.47 * local +
                                              startPhase * 1.8) +
                          0.028 * sine_lookup(2.0 * pi * baseHz * 2.63 * local +
                                              startPhase * 2.2) +
                          0.035 * sine_lookup(2.0 * pi * baseHz * 0.73 * local +
                                              startPhase * 2.7) +
                          0.022 * sine_lookup(2.0 * pi * baseHz * 2.91 * local +
                                              startPhase * 3.1));
    return intensity * (bridge + board);
}

double sympathetic_tail(
    double baseHz,
    double local,
    double intensity,
    double startPhase) {
    if (local < 0.012) {
        return 0.0;
    }
    const double tail = std::exp(-0.82 * local);
    return intensity * tail *
           (0.035 * sine_lookup(2.0 * pi * baseHz * 1.5 * local + startPhase) +
            0.021 * sine_lookup(2.0 * pi * baseHz * 2.0 * local +
                                startPhase * 1.6));
}

} // namespace

PreparedEvent Model::prepare(
    const ::mq::kernel::performance::TimedEvent& event) const {
    PreparedEvent result;
    result.onset_seconds = event.onset.decimal() * config_.seconds_per_unit;
    result.duration_seconds =
        event.duration.decimal() * config_.seconds_per_unit;
    result.intensity = std::clamp(event.intensity.decimal(), 0.0, 4.0);
    result.release_seconds = event.release
                                ? event.release->duration.decimal() *
                                      config_.seconds_per_unit
                                : 1.0;
    result.center_cents = event.target.center.cents();
    result.strike_position =
        0.11 + static_cast<double>(hash(
                          config_.seed + 23,
                          event.target.event.identity.str()) % 1900U) /
                                  10000.0;
    result.start_phase = phase(event, config_.seed, 17);
    result.decay = sustain_decay(event.articulation);
    constexpr double detuning[] = {-0.0018, 0.0, 0.0021};
    for (int course = 0; course < 3; ++course) {
        const double strikePosition =
            result.strike_position + 0.012 * static_cast<double>(course);
        for (int partial = 1; partial <= 12; ++partial) {
            const auto index = static_cast<std::size_t>(course * 12 +
                                                         partial - 1);
            const double n = static_cast<double>(partial);
            const double inharmonic = 0.00016 * n * n;
            result.mode_amplitudes[index] = std::sin(
                pi * n * strikePosition);
            result.mode_multipliers[index] =
                (1.0 + detuning[course]) * n * std::sqrt(1.0 + inharmonic);
        }
    }

    if (event.contour) {
        result.contour.reserve(event.contour->points.size());
        for (const auto& point : event.contour->points) {
            result.contour.push_back({
                point.position.decimal(),
                point.offset.cents(),
            });
        }
    }
    if (event.ornament) {
        result.approach = event.ornament->kind ==
                          ::mq::kernel::performance::OrnamentKind::Approach;
        result.oscillation = event.ornament->kind ==
                             ::mq::kernel::performance::OrnamentKind::Oscillation;
        result.ornament_onset = event.ornament->timing.onset.decimal();
        result.ornament_duration = event.ornament->timing.duration.decimal();
        result.ornament_extent_cents = event.ornament->extent.cents();
        result.ornament_cycles = event.ornament->cycles.decimal();
    }

    // The board and sympathetic terms use a declared exponential decay. A
    // -40 dB cutoff makes long performances practical without truncating an
    // audible body tail; the final render profile still owns the file tail.
    constexpr double audible_floor = 1.0e-2;
    constexpr double body_decay = 0.78;
    const double body_tail = -std::log(audible_floor) / body_decay;
    const double string_tail = result.release_seconds > 0.0
                                   ? -std::log(audible_floor) /
                                         result.decay * result.release_seconds
                                   : 0.0;
    result.audible_until_seconds = result.onset_seconds +
                                   result.duration_seconds +
                                   std::max(body_tail, string_tail);
    return result;
}

double Model::sample(const PreparedEvent& event, double seconds) const {
    const double local = seconds - event.onset_seconds;
    if (local < 0.0 || seconds > event.audible_until_seconds) {
        return 0.0;
    }
    const double position = event.duration_seconds > 0.0
                                ? std::clamp(local / event.duration_seconds,
                                             0.0, 1.0)
                                : 0.0;
    const double baseHz = frequency_hz(event, config_.tonic_hz, position);

    double strings = 0.0;
    for (int course = 0; course < 3; ++course) {
        strings += string_mode(
            baseHz,
            local,
            event.duration_seconds,
            event.release_seconds,
            event.intensity,
            event.mode_multipliers.data() + course * 12,
            event.mode_amplitudes.data() + course * 12,
            event.start_phase + 0.23 * static_cast<double>(course),
            event.decay);
    }
    return strings +
           hammer_transient(
               baseHz,
               local,
               event.intensity,
               event.strike_position,
               event.start_phase) +
           bridge_and_board(
               baseHz, local, event.intensity, event.start_phase) +
           sympathetic_tail(
               baseHz, local, event.intensity, event.start_phase);
}

} // namespace mq::synthesis::santur
