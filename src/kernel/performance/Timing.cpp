#include "mq/kernel/performance/Timing.hpp"

#include <cstdint>

namespace mq::kernel::performance {
namespace {

std::uint64_t mix(std::uint64_t seed, std::size_t ordinal) noexcept {
    auto value = seed + 0x9e3779b97f4a7c15ULL +
                 static_cast<std::uint64_t>(ordinal);
    value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31);
}

Rational rate(const Timing& timing, const Context& context) {
    if (context.phrase_end || context.cadence) {
        return timing.phrase_end_rate;
    }
    if (context.phrase_start) {
        return timing.phrase_start_rate;
    }
    return timing.phrase_body_rate;
}

Rational variation(
    const Rational& amplitude,
    const Context& context) {
    const auto bucket = mix(context.seed, context.ordinal) % 3U;
    if (bucket == 0U) return -amplitude;
    if (bucket == 2U) return amplitude;
    return Rational(0);
}

} // namespace

TimingIntent Timing::resolve(
    motion::Direction direction,
    const Context& context) const {
    const auto& base = for_direction(direction);
    const auto duration_delta = variation(duration_variation, context);
    const auto intensity_delta = variation(intensity_variation, context);
    const auto duration_factor = Rational(1) + duration_delta;
    const auto intensity_factor = Rational(1) + intensity_delta;
    return TimingIntent{
        base.duration * rate(*this, context) * duration_factor,
        base.intensity * intensity_factor,
        base.articulation,
        base.release_duration * rate(*this, context),
        base.release_articulation,
    };
}

} // namespace mq::kernel::performance
