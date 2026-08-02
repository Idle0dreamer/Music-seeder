#pragma once

#include <cstdint>
#include <string_view>

namespace app::generate {

[[nodiscard]] int run(std::uint64_t seed);
[[nodiscard]] bool is_named(std::string_view name) noexcept;
[[nodiscard]] int named(std::string_view name, std::uint64_t seed);

} // namespace app::generate
