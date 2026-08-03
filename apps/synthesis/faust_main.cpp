#include "mq/synthesis/Plan.hpp"
#include "mq/synthesis/FaustRender.hpp"
#include "mq/synthesis/Player.hpp"
#include "mq/kernel/performance/Profile.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <set>
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
    const auto preview = mq::synthesis::make_plan(
        options->maqam, options->seed, *timing);
    if (!preview) {
        std::cerr << preview.error() << '\n';
        return 1;
    }
    const auto phrase_seconds = timing->seconds_per_unit.decimal() *
                                preview->plan.end().decimal();
    if (!(phrase_seconds > 0.0)) {
        std::cerr << "timing profile produced no phrase span\n";
        return 1;
    }
    const auto repetitions = static_cast<std::size_t>(std::max(
        1.0, std::ceil(options->duration_seconds / phrase_seconds)));
    const auto generated = mq::synthesis::make_plan(
        options->maqam, options->seed, *timing, repetitions);
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
    std::set<std::string> distinct_candidates;
    for (const auto& candidate : generated->phrase_candidates) {
        distinct_candidates.insert(candidate.str());
    }
    std::cout
        << "maqam: " << options->maqam << '\n'
        << "candidate: " << generated->candidate.str() << '\n'
        << "model: Faust-generated coupled-course physical model\n"
        << "phrases: " << generated->phrase_candidates.size() << '\n'
        << "distinct_phrase_candidates: " << distinct_candidates.size() << '\n'
        << "phrase_candidates: ";
    for (std::size_t index = 0;
         index < generated->phrase_candidates.size(); ++index) {
        if (index != 0) std::cout << ',';
        std::cout << generated->phrase_candidates[index].str();
    }
    std::cout
        << '\n'
        << "events: " << generated->plan.events.size() << '\n'
        << "frames: " << rendered->frames << '\n'
        << "seconds: " << static_cast<double>(rendered->frames) /
                               static_cast<double>(options->sample_rate) << '\n'
        << "peak: " << rendered->peak << '\n'
        << "tonic_hz: " << rendered->conversion_tonic_hz << '\n'
        << "wav: " << options->output << '\n';
    std::cout << mq::synthesis::describe_plan(generated->plan);
    return 0;
}
