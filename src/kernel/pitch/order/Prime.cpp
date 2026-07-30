#include "Internal.hpp"

#include <array>
#include <cstdint>

namespace mq::kernel::pitch::order::detail {
namespace {

std::uint64_t add(
    std::uint64_t left,
    std::uint64_t right,
    std::uint64_t modulus) {
    return left >= modulus - right
             ? left - (modulus - right)
             : left + right;
}

std::uint64_t multiply(
    std::uint64_t left,
    std::uint64_t right,
    std::uint64_t modulus) {
    std::uint64_t result = 0;
    while (right != 0) {
        if ((right & 1U) != 0) {
            result = add(result, left, modulus);
        }
        right >>= 1U;
        if (right != 0) {
            left = add(left, left, modulus);
        }
    }
    return result;
}

std::uint64_t power(
    std::uint64_t base,
    std::uint64_t exponent,
    std::uint64_t modulus) {
    std::uint64_t result = 1;
    while (exponent != 0) {
        if ((exponent & 1U) != 0) {
            result = multiply(result, base, modulus);
        }
        exponent >>= 1U;
        if (exponent != 0) {
            base = multiply(base, base, modulus);
        }
    }
    return result;
}

} // namespace

bool prime(std::int64_t value) {
    constexpr std::array<std::uint64_t, 12> small{
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
    };
    if (value < 2) {
        return false;
    }
    const auto candidate = static_cast<std::uint64_t>(value);
    for (const auto divisor : small) {
        if (candidate == divisor) {
            return true;
        }
        if (candidate % divisor == 0) {
            return false;
        }
    }

    auto odd = candidate - 1;
    std::uint64_t shifts = 0;
    while ((odd & 1U) == 0) {
        odd >>= 1U;
        ++shifts;
    }
    constexpr std::array<std::uint64_t, 7> witnesses{
        2, 325, 9'375, 28'178, 450'775, 9'780'504, 1'795'265'022,
    };
    for (const auto witness : witnesses) {
        if (witness % candidate == 0) {
            continue;
        }
        auto residue = power(witness % candidate, odd, candidate);
        if (residue == 1 || residue == candidate - 1) {
            continue;
        }
        bool passed = false;
        for (std::uint64_t step = 1; step < shifts; ++step) {
            residue = multiply(residue, residue, candidate);
            if (residue == candidate - 1) {
                passed = true;
                break;
            }
        }
        if (!passed) {
            return false;
        }
    }
    return true;
}

} // namespace mq::kernel::pitch::order::detail
