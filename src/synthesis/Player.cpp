#include "mq/synthesis/Player.hpp"

#include <charconv>
#include <cstdlib>
#include <limits>
#include <string_view>

namespace mq::synthesis {
namespace {

constexpr std::string_view default_timing =
    "theory/data/performance/continuous-free-rhythm-v1.timing";
constexpr std::uint32_t default_sample_rate = 48'000;
constexpr double default_tonic_hz = 146.8323839587;
constexpr double default_duration_seconds = 30.0;

std::expected<std::uint64_t, std::string> unsigned_value(
    std::string_view value,
    std::string_view label) {
    std::uint64_t result{};
    const auto parsed = std::from_chars(
        value.data(), value.data() + value.size(), result);
    if (parsed.ec != std::errc{} ||
        parsed.ptr != value.data() + value.size()) {
        return std::unexpected(std::string(label) + " must be an unsigned integer");
    }
    return result;
}

std::expected<double, std::string> real_value(
    std::string_view value,
    std::string_view label) {
    std::string owned(value);
    char* end{};
    const double result = std::strtod(owned.c_str(), &end);
    if (end != owned.c_str() + owned.size() || result <= 0.0) {
        return std::unexpected(std::string(label) + " must be positive");
    }
    return result;
}

} // namespace

std::expected<PlayerConfig, std::string> parse_player_args(
    int argc,
    char** argv,
    const std::string& program) {
    if (argc == 2 && std::string_view(argv[1]) == "--help") {
        return std::unexpected(
            "usage: " + program +
            " --maqam name --seed n [--output path] [--timing path]"
            " [--sample-rate hz] [--tonic-hz hz] [--duration seconds]");
    }
    PlayerConfig result{
        .maqam = {},
        .seed = 0,
        .timing_path = std::string(default_timing),
        .output = {},
        .sample_rate = default_sample_rate,
        .tonic_hz = default_tonic_hz,
        .duration_seconds = default_duration_seconds,
    };
    for (int index = 1; index < argc; ++index) {
        const std::string_view option(argv[index]);
        if (index + 1 >= argc) {
            return std::unexpected("missing value for " + std::string(option));
        }
        const std::string_view value(argv[++index]);
        if (option == "--maqam") {
            result.maqam = value;
        } else if (option == "--seed") {
            const auto parsed = unsigned_value(value, "--seed");
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            result.seed = *parsed;
        } else if (option == "--output") {
            result.output = value;
        } else if (option == "--timing") {
            result.timing_path = value;
        } else if (option == "--sample-rate") {
            const auto parsed = unsigned_value(value, "--sample-rate");
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            if (*parsed == 0 ||
                *parsed > std::numeric_limits<std::uint32_t>::max()) {
                return std::unexpected("--sample-rate is out of range");
            }
            result.sample_rate = static_cast<std::uint32_t>(*parsed);
        } else if (option == "--tonic-hz") {
            const auto parsed = real_value(value, "--tonic-hz");
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            result.tonic_hz = *parsed;
        } else if (option == "--duration") {
            const auto parsed = real_value(value, "--duration");
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            if (*parsed > 90.0) {
                return std::unexpected("--duration may not exceed 90 seconds");
            }
            result.duration_seconds = *parsed;
        } else {
            return std::unexpected("unknown option: " + std::string(option));
        }
    }
    if (result.maqam.empty()) {
        return std::unexpected("--maqam is required");
    }
    if (result.output.empty()) {
        result.output = "build/" + result.maqam + "-" +
                        std::to_string(result.seed) + ".wav";
    }
    return result;
}

} // namespace mq::synthesis
