#include "../Test.hpp"

#include "mq/kernel/pitch/trajectory/Model.hpp"

void test::trajectory() {
    using namespace mq::kernel::pitch::trajectory;

    const Model model{
        .base = {
            {{1.0, Basis{Basis::Stable{}}}},
        },
        .ascent = {
            {{0.2, Basis{Basis::Onset{0.1}}}},
        },
        .descent = {},
        .cadence_approach = {
            {{-0.1, Basis{Basis::Release{0.2}}}},
        },
        .emphasis_attraction = {
            {{0.05, Basis{Basis::Attraction{1.0, 2.0}}}},
        },
    };

    const auto neutral = model.resolve({false, false, false, false});
    require(neutral.evaluate(0.5) == 1.0, "neutral evaluation failed");

    const auto ascending = model.resolve({true, false, false, false});
    require(ascending.evaluate(0.5) > 1.0, "ascent evaluation failed");

    const auto cadence = model.resolve({false, false, true, false});
    require(cadence.evaluate(0.9) < 1.0, "cadence release failed");
}
