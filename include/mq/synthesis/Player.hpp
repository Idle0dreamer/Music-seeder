#pragma once

#include <cstdint>
#include <expected>
#include <string>

namespace mq::synthesis {

struct PlayerConfig {
    std::string maqam;
    std::uint64_t seed{};
    std::string timing_path;
    std::string output;
    std::uint32_t sample_rate{};
    double tonic_hz{};
    double duration_seconds{};
};

[[nodiscard]] std::expected<PlayerConfig, std::string> parse_player_args(
    int argc,
    char** argv,
    const std::string& program);

} // namespace mq::synthesis
