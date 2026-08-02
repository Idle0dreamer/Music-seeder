#include "mq/synthesis/Bayati.hpp"
#include "mq/synthesis/Render.hpp"
#include "mq/kernel/performance/Profile.hpp"

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "usage: synthesis-render seed [output.wav]\n";
        return 2;
    }
    std::uint64_t seed{};
    const std::string_view seedValue(argv[1]);
    const auto parsed = std::from_chars(
        seedValue.data(),
        seedValue.data() + seedValue.size(),
        seed);
    if (parsed.ec != std::errc{} ||
        parsed.ptr != seedValue.data() + seedValue.size()) {
        std::cerr << "seed must be an unsigned integer\n";
        return 2;
    }
    const std::string output = argc == 3
                                 ? argv[2]
                                 : "build/bayati-" + std::to_string(seed) + ".wav";

    const auto timing = mq::kernel::performance::load_timing_profile(
        "theory/data/performance/free-rhythm-v1.timing");
    if (!timing) {
        std::cerr << timing.error() << '\n';
        return 1;
    }
    const auto generated = mq::synthesis::make_bayati_plan(seed, *timing);
    if (!generated) {
        std::cerr << generated.error() << '\n';
        return 1;
    }
    const auto rendered = mq::synthesis::render_wav(
        generated->plan,
        mq::synthesis::RenderConfig{
            .seconds_per_unit = timing->seconds_per_unit.decimal(),
            .tail_seconds = timing->tail_seconds.decimal(),
            .seed = seed,
        },
        output);
    if (!rendered) {
        std::cerr << rendered.error().message << '\n';
        return 1;
    }
    std::cout
        << "candidate: " << generated->candidate.str() << '\n'
        << "frames: " << rendered->frames << '\n'
        << "peak: " << rendered->peak << '\n'
        << "model: provisional santur coupled-course physical model\n"
        << "tonic_hz: " << rendered->conversion_tonic_hz << '\n'
        << "wav: " << output << '\n';
    return 0;
}
