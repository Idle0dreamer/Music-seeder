#include "../Test.hpp"

mq::kernel::performance::Timing test::timing_profile() {
    const auto profile = mq::kernel::performance::load_timing_profile(
        "theory/data/performance/free-rhythm-v1.timing");
    if (!profile) {
        throw std::runtime_error(profile.error());
    }
    return *profile;
}
