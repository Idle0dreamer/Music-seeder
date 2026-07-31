#include "mq/kernel/pitch/trajectory/Basis.hpp"

#include <numbers>

namespace mq::kernel::pitch::trajectory {

double Basis::evaluate(double t) const noexcept {
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    if (std::isnan(t) || std::isinf(t)) {
        return 0.0; // fallback for invalid time
    }
    return std::visit(
        [t](const auto& form) -> double {
            using T = std::decay_t<decltype(form)>;
            if constexpr (std::is_same_v<T, Stable>) {
                return 1.0;
            } else if constexpr (std::is_same_v<T, Onset>) {
                if (form.duration <= 0.0) return 1.0;
                if (t >= form.duration) return 1.0;
                // smoothstep onset
                const double x = t / form.duration;
                return x * x * (3.0 - 2.0 * x);
            } else if constexpr (std::is_same_v<T, Release>) {
                if (form.duration <= 0.0) return 1.0;
                const double start = 1.0 - form.duration;
                if (t <= start) return 1.0;
                // smoothstep release
                const double x = (1.0 - t) / form.duration;
                return x * x * (3.0 - 2.0 * x);
            } else if constexpr (std::is_same_v<T, Vibrato>) {
                const double rate = form.rate.value;
                const double extent = form.extent.value;
                if (std::isnan(rate) || std::isinf(rate) || std::isnan(extent) || std::isinf(extent)) return 0.0;
                return extent * std::sin(2.0 * std::numbers::pi * rate * t);
            } else if constexpr (std::is_same_v<T, Attraction>) {
                const double amount = form.amount.value;
                if (std::isnan(amount) || std::isinf(amount) || std::isnan(form.skew) || std::isinf(form.skew)) return 0.0;
                // simple polynomial skew
                const double x = std::pow(t, std::exp(form.skew));
                return amount * x;
            } else {
                return 0.0;
            }
        },
        form);
}

} // namespace mq::kernel::pitch::trajectory
