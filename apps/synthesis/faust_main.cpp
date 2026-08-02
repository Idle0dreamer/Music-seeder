#include "mq/synthesis/Plan.hpp"
#include "mq/synthesis/FaustRender.hpp"
#include "mq/synthesis/Player.hpp"
#include "mq/kernel/performance/Profile.hpp"

#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    const auto options = mq::synthesis::parse_player_args(
        argc, argv, "synthesis-faust-render");
    if (!options) {
        std::cerr << options.error() << '\n';
        return argc == 2 && std::string_view(argv[1]) == "--help" ? 0 : 2;
    }
    const auto timing = mq::kernel::performance::load_timing_profile(
        options->timing_path);
    if (!timing) {
        std::cerr << timing.error() << '\n';
        return 1;
    }
    const auto generated = mq::synthesis::make_plan(
        options->maqam, options->seed, *timing);
    if (!generated) {
        std::cerr << generated.error() << '\n';
        return 1;
    }
    const auto rendered = mq::synthesis::render_faust_wav(
        generated->plan,
        mq::synthesis::RenderConfig{
            .sample_rate = options->sample_rate,
            .tonic_hz = options->tonic_hz,
            .seconds_per_unit = timing->seconds_per_unit.decimal(),
            .tail_seconds = timing->tail_seconds.decimal(),
            .seed = options->seed,
        },
        options->output);
    if (!rendered) {
        std::cerr << rendered.error().message << '\n';
        return 1;
    }
    std::cout
        << "maqam: " << options->maqam << '\n'
        << "candidate: " << generated->candidate.str() << '\n'
        << "model: Faust-generated coupled-course physical model\n"
        << "frames: " << rendered->frames << '\n'
        << "peak: " << rendered->peak << '\n'
        << "tonic_hz: " << rendered->conversion_tonic_hz << '\n'
        << "wav: " << options->output << '\n';
    return 0;
}
