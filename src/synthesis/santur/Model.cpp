#include "mq/synthesis/santur/Model.hpp"
#include "mq/synthesis/Pitch.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numbers>
#include <string>

namespace mq::synthesis::santur {
namespace {

constexpr double pi = std::numbers::pi_v<double>;

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

double string_mode(
    double baseHz,
    double local,
    double duration,
    double intensity,
    double detune,
    double strikePosition,
    double startPhase,
    double decay) {
    const double stringHz = baseHz * (1.0 + detune);
    double result = 0.0;
    for (int partial = 1; partial <= 6; ++partial) {
        const double n = static_cast<double>(partial);
        // Upper modes are dispersive rather than exact integer multiples.
        const double inharmonic = 0.00022 * n * n;
        const double modeHz = stringHz * n * std::sqrt(1.0 + inharmonic);
        const double position = std::sin(pi * n * strikePosition);
        const double amplitude = (0.78 / n) * position;
        const double modeDecay = decay + 0.28 * n +
                                 1.5 * std::max(0.0, n - 3.0);
        result += amplitude * std::exp(-modeDecay * local) *
                  std::sin(2.0 * pi * modeHz * local + startPhase * n);
    }

    const double contact = 0.0035 + 0.0025 / (1.0 + intensity);
    const double contactPhase = std::clamp(local / contact, 0.0, 1.0);
    const double hammer = local < contact
                              ? std::sin(pi * contactPhase)
                              : 1.0;
    const double release = local < duration
                               ? 1.0
                               : std::exp(-decay * (local - duration));
    return intensity * 0.24 * hammer * result * release;
}

double bridge_and_board(
    double baseHz,
    double local,
    double intensity,
    double startPhase) {
    const double bridge = 0.11 * std::exp(-4.2 * local) *
                          std::sin(2.0 * pi * baseHz * 1.003 * local +
                                   startPhase * 0.7);
    const double boardEnvelope = std::exp(-1.05 * local);
    const double board = boardEnvelope *
                         (0.06 * std::sin(2.0 * pi * baseHz * 0.51 * local +
                                          startPhase * 1.3) +
                          0.045 * std::sin(2.0 * pi * baseHz * 1.47 * local +
                                           startPhase * 1.8) +
                          0.028 * std::sin(2.0 * pi * baseHz * 2.63 * local +
                                           startPhase * 2.2));
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
           (0.035 * std::sin(2.0 * pi * baseHz * 1.5 * local + startPhase) +
            0.021 * std::sin(2.0 * pi * baseHz * 2.0 * local +
                              startPhase * 1.6));
}

} // namespace

double Model::sample(
    const ::mq::kernel::performance::TimedEvent& event,
    double seconds) const {
    const double onset = event.onset.decimal() * config_.seconds_per_unit;
    const double duration = event.duration.decimal() * config_.seconds_per_unit;
    const double local = seconds - onset;
    if (local < 0.0) {
        return 0.0;
    }

    const double position = duration > 0.0
                                ? std::clamp(local / duration, 0.0, 1.0)
                                : 0.0;
    const double baseHz = ::mq::synthesis::pitch::frequency_hz(
        event, config_.tonic_hz, position);
    const double intensity = std::clamp(event.intensity.decimal(), 0.0, 4.0);
    const double startPhase = phase(event, config_.seed, 17);
    const double strikePosition = 0.11 +
                                   static_cast<double>(hash(
                                       config_.seed + 23,
                                       event.target.event.identity.str()) %
                                                       1900U) /
                                       10000.0;
    const double decay = sustain_decay(event.articulation);

    double strings = 0.0;
    constexpr double detuning[] = {-0.0018, 0.0, 0.0021};
    for (int course = 0; course < 3; ++course) {
        strings += string_mode(
            baseHz,
            local,
            duration,
            intensity,
            detuning[course],
            strikePosition + 0.012 * static_cast<double>(course),
            startPhase + 0.23 * static_cast<double>(course),
            decay);
    }
    return strings + bridge_and_board(baseHz, local, intensity, startPhase) +
           sympathetic_tail(baseHz, local, intensity, startPhase);
}

} // namespace mq::synthesis::santur
