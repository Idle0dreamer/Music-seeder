#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Bayati.hpp"
#include "mq/synthesis/Render.hpp"

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

    const auto scaffold = mq::kernel::maqam::make_bayati();
    if (!scaffold) {
        std::cerr << scaffold.error() << '\n';
        return 1;
    }
    const mq::kernel::eval::Context context{
        .jins = {&scaffold->ajnas},
        .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr},
        .grammar = {},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    const auto generated = engine.run(
        seed,
        scaffold->generation.choice,
        scaffold->generation.production,
        scaffold->generation.projection,
        scaffold->generation.schema);
    if (!generated) {
        std::cerr << generated.error().message << '\n';
        return 1;
    }
    const auto selected = std::ranges::find(
        generated->legal,
        generated->selected,
        &mq::kernel::generate::Outcome::candidate);
    if (selected == generated->legal.end()) {
        std::cerr << "selected Bayati outcome is missing\n";
        return 1;
    }
    const auto rendered = mq::synthesis::render_wav(
        selected->plan,
        mq::synthesis::RenderConfig{.seed = seed},
        output);
    if (!rendered) {
        std::cerr << rendered.error().message << '\n';
        return 1;
    }
    std::cout
        << "candidate: " << generated->selected.str() << '\n'
        << "frames: " << rendered->frames << '\n'
        << "peak: " << rendered->peak << '\n'
        << "model: provisional santur coupled-course physical model\n"
        << "tonic_hz: " << rendered->conversion_tonic_hz << '\n'
        << "wav: " << output << '\n';
    return 0;
}
