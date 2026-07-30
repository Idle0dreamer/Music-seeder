#include "Internal.hpp"

#include <limits>
#include <numeric>

namespace mq::kernel::pitch::order::detail {

Powers normalize(const Expression& left, const Expression& right) {
    Powers powers = left.powers();
    for (const auto& [prime, exponent] : right.powers()) {
        powers[prime] -= exponent;
    }
    powers[2] +=
        (left.offset() - right.offset()) / Rational(1200);
    std::erase_if(
        powers,
        [](const auto& item) { return item.second == Rational(0); });
    return powers;
}

std::expected<std::uint64_t, Error> scale(const Powers& powers) {
    std::uint64_t result = 1;
    for (const auto& [prime, exponent] : powers) {
        static_cast<void>(prime);
        const auto denominator =
            static_cast<std::uint64_t>(exponent.denominator());
        const auto divisor = std::gcd(result, denominator);
        const auto factor = denominator / divisor;
        if (result > std::numeric_limits<std::uint64_t>::max() / factor) {
            return std::unexpected(Error{
                Error::Code::Scale,
                "pitch order denominator LCM exceeds exact scale capacity",
            });
        }
        result *= factor;
    }
    return result;
}

} // namespace mq::kernel::pitch::order::detail
