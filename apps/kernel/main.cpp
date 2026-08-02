#include "generate/Run.hpp"
#include "pitch/Run.hpp"

#include <charconv>
#include <cstdint>
#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    const bool bayati = argc > 1 && std::string_view(argv[1]) == "bayati";
    if ((!bayati && argc > 2) || (bayati && argc > 3)) {
        std::cerr << "usage: kernel [seed] | kernel bayati [seed]\n";
        return 2;
    }
    std::uint64_t seed{};
    const int seedArgument = bayati ? 2 : 1;
    if (argc > seedArgument) {
        const std::string_view value(argv[seedArgument]);
        const auto parsed = std::from_chars(
            value.data(),
            value.data() + value.size(),
            seed);
        if (parsed.ec != std::errc{} ||
            parsed.ptr != value.data() + value.size()) {
            std::cerr << "seed must be an unsigned integer\n";
            return 2;
        }
    }
    const auto pitch = app::pitch::run();
    if (pitch != 0) {
        return pitch;
    }
    return bayati ? app::generate::bayati(seed) : app::generate::run(seed);
}
