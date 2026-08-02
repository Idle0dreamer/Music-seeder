#pragma once

#include "mq/kernel/performance/Timing.hpp"

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>

namespace mq::kernel::performance {

[[nodiscard]] std::expected<Timing, std::string> parse_timing_profile(
    std::string_view text);

[[nodiscard]] std::expected<Timing, std::string> load_timing_profile(
    const std::filesystem::path& path);

} // namespace mq::kernel::performance
