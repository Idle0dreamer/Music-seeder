#include "generate/Run.hpp"
#include "pitch/Run.hpp"

#include <charconv>
#include <cstdint>
#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    const auto mode = argc > 1 ? std::string_view(argv[1]) : std::string_view{};
    const bool named = app::generate::is_named(mode);
    if ((!named && argc > 2) || (named && argc > 3)) {
        std::cerr << "usage: kernel [seed] | kernel <maqam> [seed]\n";
        return 2;
    }
    std::uint64_t seed{};
    const int seedArgument = named ? 2 : 1;
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
    if (named) {
        return app::generate::named(mode, seed);
    }
    return app::generate::run(seed);
}
