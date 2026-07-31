#include "mq/kernel/pitch/trajectory/Basis.hpp"

#include <numbers>

namespace mq::kernel::pitch::trajectory {

double Basis::evaluate(double t) const noexcept {
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    return std::visit(
        [t](const auto& function) -> double {
            using T = std::decay_t<decltype(function)>;
            if constexpr (std::is_same_v<T, Stable>) {
                return 1.0;
            } else if constexpr (std::is_same_v<T, Onset>) {
                if (function.duration <= 0.0) return 1.0;
                if (t >= function.duration) return 1.0;
                // smoothstep onset
                const double x = t / function.duration;
                return x * x * (3.0 - 2.0 * x);
            } else if constexpr (std::is_same_v<T, Release>) {
                if (function.duration <= 0.0) return 1.0;
                const double start = 1.0 - function.duration;
                if (t <= start) return 1.0;
                // smoothstep release
                const double x = (1.0 - t) / function.duration;
                return x * x * (3.0 - 2.0 * x);
            } else if constexpr (std::is_same_v<T, Vibrato>) {
                return function.extent * std::sin(2.0 * std::numbers::pi * function.rate * t);
            } else if constexpr (std::is_same_v<T, Attraction>) {
                // simple polynomial skew
                const double x = std::pow(t, std::exp(function.skew));
                return function.amount * x;
            } else {
                return 0.0;
            }
        },
        form);
}

} // namespace mq::kernel::pitch::trajectory
