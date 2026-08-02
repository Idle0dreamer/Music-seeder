#include "mq/synthesis/Render.hpp"
#include "mq/synthesis/santur/Model.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <vector>

namespace mq::synthesis {
namespace {

void little(std::ofstream& output, std::uint32_t value) {
    for (int byte = 0; byte < 4; ++byte) {
        output.put(static_cast<char>((value >> (byte * 8)) & 0xffU));
    }
}

void little(std::ofstream& output, std::uint16_t value) {
    output.put(static_cast<char>(value & 0xffU));
    output.put(static_cast<char>((value >> 8) & 0xffU));
}

void header(
    std::ofstream& output,
    std::uint32_t sampleRate,
    std::uint32_t frames) {
    const std::uint32_t bytes = frames * sizeof(std::int16_t);
    output.write("RIFF", 4);
    little(output, 36U + bytes);
    output.write("WAVEfmt ", 8);
    little(output, 16U);
    little(output, static_cast<std::uint16_t>(1));
    little(output, static_cast<std::uint16_t>(1));
    little(output, sampleRate);
    little(output, static_cast<std::uint32_t>(
        sampleRate * sizeof(std::int16_t)));
    little(output, static_cast<std::uint16_t>(sizeof(std::int16_t)));
    little(output, static_cast<std::uint16_t>(16));
    output.write("data", 4);
    little(output, bytes);
}

} // namespace

std::expected<RenderReport, RenderError> render_wav(
    const ::mq::kernel::performance::Plan& plan,
    const RenderConfig& config,
    const std::string& path) {
    if (!plan.well_formed()) {
        return std::unexpected(RenderError{
            "synthesis rejected a malformed performance plan",
        });
    }
    if (config.sample_rate == 0 || config.tonic_hz <= 0.0 ||
        config.seconds_per_unit <= 0.0 || config.tail_seconds < 0.0 ||
        config.seconds_per_unit * plan.end().decimal() +
                config.tail_seconds > 120.0) {
        return std::unexpected(RenderError{
            "synthesis render configuration is outside its declared bounds",
        });
    }

    const double seconds =
        config.seconds_per_unit * plan.end().decimal() + config.tail_seconds;
    const auto frames = static_cast<std::uint64_t>(
        std::ceil(seconds * static_cast<double>(config.sample_rate)));
    if (frames == 0 || frames > std::numeric_limits<std::uint32_t>::max()) {
        return std::unexpected(RenderError{"WAV frame count is unsupported"});
    }

    const santur::Model model({
        config.sample_rate,
        config.tonic_hz,
        config.seconds_per_unit,
        config.seed,
    });
    std::vector<double> samples(frames);
    std::vector<santur::PreparedEvent> voices;
    voices.reserve(plan.events.size());
    for (const auto& event : plan.events) {
        voices.push_back(model.prepare(event));
    }
    for (const auto& voice : voices) {
        const auto onset = static_cast<std::uint64_t>(std::ceil(
            std::max(0.0, voice.onset_seconds) *
            static_cast<double>(config.sample_rate)));
        const auto audibleUntil = static_cast<std::uint64_t>(std::ceil(
            std::max(0.0, voice.audible_until_seconds) *
            static_cast<double>(config.sample_rate)));
        const auto end = std::min(frames, audibleUntil);
        for (std::uint64_t frame = onset; frame < end; ++frame) {
            const double at = static_cast<double>(frame) /
                              static_cast<double>(config.sample_rate);
            samples[frame] += model.sample(voice, at);
        }
    }
    double peak = 0.0;
    for (const double sample : samples) {
        peak = std::max(peak, std::abs(sample));
    }
    if (!(peak > 0.0) || !std::isfinite(peak)) {
        return std::unexpected(RenderError{"render produced no finite signal"});
    }

    return write_wav_samples(samples, config, path);
}

std::expected<RenderReport, RenderError> write_wav_samples(
    std::span<const double> samples,
    const RenderConfig& config,
    const std::string& path) {
    if (config.sample_rate == 0 || config.tonic_hz <= 0.0 ||
        config.seconds_per_unit <= 0.0 || config.tail_seconds < 0.0 ||
        samples.empty() || samples.size() >
            std::numeric_limits<std::uint32_t>::max()) {
        return std::unexpected(RenderError{
            "WAV sample export configuration is outside its declared bounds",
        });
    }
    double peak = 0.0;
    for (const double sample : samples) {
        if (!std::isfinite(sample)) {
            return std::unexpected(RenderError{"render produced a non-finite sample"});
        }
        peak = std::max(peak, std::abs(sample));
    }
    if (!(peak > 0.0)) {
        return std::unexpected(RenderError{"render produced no finite signal"});
    }

    const auto frames = static_cast<std::uint32_t>(samples.size());
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) {
        return std::unexpected(RenderError{"could not open WAV output: " + path});
    }
    header(output, config.sample_rate, static_cast<std::uint32_t>(frames));
    const double gain = 0.92 / peak;
    double normalizedPeak = 0.0;
    for (const double sample : samples) {
        const double normalized = std::clamp(sample * gain, -0.92, 0.92);
        normalizedPeak = std::max(normalizedPeak, std::abs(normalized));
        const auto pcm = static_cast<std::int16_t>(std::lrint(
            normalized * static_cast<double>(std::numeric_limits<std::int16_t>::max())));
        little(output, static_cast<std::uint16_t>(pcm));
    }
    if (!output) {
        return std::unexpected(RenderError{"WAV output failed while writing: " + path});
    }
    return RenderReport{frames, normalizedPeak, config.tonic_hz};
}

} // namespace mq::synthesis
