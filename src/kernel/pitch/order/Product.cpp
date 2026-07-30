#include "Internal.hpp"

#include <limits>

namespace mq::kernel::pitch::order::detail {
namespace {

bool exceeds(
    std::size_t left,
    std::size_t right,
    std::size_t limit) {
    if (left == 0 || right == 0) {
        return false;
    }
    if (left > limit || right > limit) {
        return true;
    }
    return left > limit - right + 1U;
}

std::expected<std::uint64_t, Error> magnitude(
    const Rational& value,
    std::uint64_t common,
    Limits limits) {
    const auto signedNumerator = value.numerator();
    const auto numerator = signedNumerator < 0
                             ? static_cast<std::uint64_t>(
                                   -(signedNumerator + 1)) +
                                   1U
                             : static_cast<std::uint64_t>(signedNumerator);
    const auto factor =
        common / static_cast<std::uint64_t>(value.denominator());
    if (numerator != 0 &&
        factor > std::numeric_limits<std::uint64_t>::max() / numerator) {
        return std::unexpected(Error{
            Error::Code::Exponent,
            "pitch order exponent exceeds exact integer capacity",
        });
    }
    const auto exponent = factor * numerator;
    if (exponent > limits.exponent) {
        return std::unexpected(Error{
            Error::Code::Exponent,
            "pitch order exponent exceeds proof budget",
        });
    }
    return exponent;
}

} // namespace

std::expected<math::Natural, Error> product(
    const Powers& powers,
    std::uint64_t common,
    bool positive,
    Limits limits) {
    math::Natural result(1);
    for (const auto& [basis, value] : powers) {
        if (!prime(basis)) {
            return std::unexpected(Error{
                Error::Code::Prime,
                "pitch order contains an invalid prime basis",
            });
        }
        if ((value.numerator() > 0) != positive) {
            continue;
        }
        const auto exponent = magnitude(value, common, limits);
        if (!exponent) {
            return std::unexpected(exponent.error());
        }
        const auto factor = math::power(
            math::Natural(static_cast<std::uint64_t>(basis)),
            *exponent,
            limits.product);
        if (!factor ||
            exceeds(result.bits(), factor->bits(), limits.product)) {
            return std::unexpected(Error{
                Error::Code::Product,
                "pitch order integer product exceeds proof budget",
            });
        }
        result *= *factor;
        if (result.bits() > limits.product) {
            return std::unexpected(Error{
                Error::Code::Product,
                "pitch order integer product exceeds proof budget",
            });
        }
    }
    return result;
}

} // namespace mq::kernel::pitch::order::detail
