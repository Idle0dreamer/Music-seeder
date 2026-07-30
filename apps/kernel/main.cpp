#include "generate/Run.hpp"
#include "pitch/Run.hpp"

#include <charconv>
#include <cstdint>
#include <iostream>
#include <string_view>

int main(int argc, char** argv) {
    if (argc > 2) {
        std::cerr << "usage: kernel [seed]\n";
        return 2;
    }
    std::uint64_t seed{};
    if (argc == 2) {
        const std::string_view value(argv[1]);
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
    return pitch == 0 ? app::generate::run(seed) : pitch;
}
