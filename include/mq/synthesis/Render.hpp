#pragma once

#include "mq/kernel/performance/Plan.hpp"

#include <cstdint>
#include <expected>
#include <span>
#include <string>

namespace mq::synthesis {

struct RenderConfig {
    std::uint32_t sample_rate{};
    double tonic_hz{};
    double seconds_per_unit{};
    double tail_seconds{};
    std::uint64_t seed{};
};

struct RenderError {
    std::string message;
};

struct RenderReport {
    std::uint64_t frames{};
    double peak{};
    double conversion_tonic_hz{};
};

[[nodiscard]] std::expected<RenderReport, RenderError> render_wav(
    const ::mq::kernel::performance::Plan& plan,
    const RenderConfig& config,
    const std::string& path);

[[nodiscard]] std::expected<RenderReport, RenderError> write_wav_samples(
    std::span<const double> samples,
    const RenderConfig& config,
    const std::string& path);

} // namespace mq::synthesis
