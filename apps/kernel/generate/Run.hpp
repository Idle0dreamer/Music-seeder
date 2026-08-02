#pragma once

#include <cstdint>

namespace app::generate {

[[nodiscard]] int run(std::uint64_t seed);
[[nodiscard]] int bayati(std::uint64_t seed);

} // namespace app::generate
