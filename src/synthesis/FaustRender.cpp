#include "mq/synthesis/FaustRender.hpp"

#include "faust/gui/APIUI.h"
#include "music_seed_santur_courses.cpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

namespace mq::synthesis {
namespace {

constexpr double max_render_seconds = 120.0;

std::expected<std::uint64_t, RenderError> frame_count(
    const ::mq::kernel::performance::Plan& plan,
    const RenderConfig& config) {
    if (!plan.well_formed()) {
        return std::unexpected(RenderError{
            "Faust adapter rejected a malformed performance plan",
        });
    }
    const double seconds =
        config.seconds_per_unit * plan.end().decimal() + config.tail_seconds;
    if (config.sample_rate == 0 || config.tonic_hz <= 0.0 ||
        config.seconds_per_unit <= 0.0 || config.tail_seconds < 0.0 ||
        seconds > max_render_seconds) {
        return std::unexpected(RenderError{
            "Faust adapter configuration is outside its declared bounds",
        });
    }
    const auto frames = static_cast<std::uint64_t>(std::ceil(
        seconds * static_cast<double>(config.sample_rate)));
    if (frames == 0 || frames > std::numeric_limits<std::uint32_t>::max()) {
        return std::unexpected(RenderError{"Faust frame count is unsupported"});
    }
    return frames;
}

bool set_parameter(APIUI& ui, const char* label, float value) {
    const int index = ui.getParamIndex(label);
    if (index < 0) {
        return false;
    }
    ui.setParamValue(index, value);
    return true;
}

} // namespace

std::expected<RenderReport, RenderError> render_faust_wav(
    const ::mq::kernel::performance::Plan& plan,
    const RenderConfig& config,
    const std::string& path) {
    const auto frames = frame_count(plan, config);
    if (!frames) {
        return std::unexpected(frames.error());
    }

    music_seed_santur_courses dsp;
    if (dsp.getNumInputs() != 0 || dsp.getNumOutputs() != 1) {
        return std::unexpected(RenderError{
            "Faust santur source must expose zero inputs and one output",
        });
    }
    dsp.init(static_cast<int>(config.sample_rate));
    APIUI ui;
    dsp.buildUserInterface(&ui);
    if (ui.getParamIndex("fundamental_hz") < 0 ||
        ui.getParamIndex("intensity") < 0 ||
        ui.getParamIndex("strike") < 0) {
        return std::unexpected(RenderError{
            "Faust santur source omitted a required continuous control",
        });
    }

    std::vector<double> samples(*frames);
    std::vector<std::uint64_t> onsets;
    onsets.reserve(plan.events.size());
    for (const auto& event : plan.events) {
        const double frequency = config.tonic_hz * std::exp2(
            event.target.center.cents() / 1200.0);
        if (!std::isfinite(frequency) || frequency < 20.0 || frequency > 2000.0) {
            return std::unexpected(RenderError{
                "Faust adapter cannot realize a frequency outside its declared range",
            });
        }
        if (event.articulation !=
                ::mq::kernel::performance::Articulation::Neutral) {
            return std::unexpected(RenderError{
                "Faust santur source does not yet realize non-neutral articulation",
            });
        }
        const auto onset = static_cast<std::uint64_t>(std::llround(
            event.onset.decimal() * config.seconds_per_unit *
            static_cast<double>(config.sample_rate)));
        onsets.push_back(onset);
    }

    std::size_t next = 0;
    for (std::uint64_t frame = 0; frame < *frames; ++frame) {
        bool strike = false;
        while (next < plan.events.size() && onsets[next] <= frame) {
            const auto& event = plan.events[next];
            const double frequency = config.tonic_hz * std::exp2(
                event.target.center.cents() / 1200.0);
            const float intensity = static_cast<float>(std::clamp(
                event.intensity.decimal() / 4.0, 0.0, 1.0));
            if (!set_parameter(ui, "fundamental_hz",
                               static_cast<float>(frequency)) ||
                !set_parameter(ui, "intensity", intensity) ||
                !set_parameter(ui, "strike", 1.0F)) {
                return std::unexpected(RenderError{
                    "Faust adapter failed to set an event control",
                });
            }
            strike = true;
            ++next;
        }
        if (!strike && !set_parameter(ui, "strike", 0.0F)) {
            return std::unexpected(RenderError{
                "Faust adapter failed to clear strike control",
            });
        }
        FAUSTFLOAT output{};
        FAUSTFLOAT* outputs[] = {&output};
        dsp.compute(1, nullptr, outputs);
        samples[frame] = static_cast<double>(output);
        if (strike && !set_parameter(ui, "strike", 0.0F)) {
            return std::unexpected(RenderError{
                "Faust adapter failed to release strike control",
            });
        }
    }
    return write_wav_samples(samples, config, path);
}

} // namespace mq::synthesis
