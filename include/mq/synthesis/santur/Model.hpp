#pragma once

#include "mq/kernel/performance/TimedEvent.hpp"

#include <cstdint>

namespace mq::synthesis::santur {

struct Config {
    std::uint32_t sample_rate{48'000};
    double tonic_hz{146.8323839587};
    double seconds_per_unit{};
    std::uint64_t seed{};
};

class Model {
public:
    explicit Model(Config config) : config_(config) {}

    [[nodiscard]] double sample(
        const ::mq::kernel::performance::TimedEvent& event,
        double seconds) const;

private:
    Config config_;
};

} // namespace mq::synthesis::santur
