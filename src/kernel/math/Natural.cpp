#include "mq/kernel/math/Natural.hpp"

#include <bit>
#include <iomanip>
#include <sstream>

namespace mq::kernel::math {
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

} // namespace

Natural::Natural(std::uint64_t value) {
    limbs_.push_back(static_cast<std::uint32_t>(value));
    const auto high = static_cast<std::uint32_t>(value >> 32U);
    if (high != 0) {
        limbs_.push_back(high);
    }
}

std::size_t Natural::bits() const noexcept {
    if (limbs_.size() == 1 && limbs_.front() == 0) {
        return 0;
    }
    return (limbs_.size() - 1) * 32U +
           std::bit_width(limbs_.back());
}

void Natural::normalize() {
    while (limbs_.size() > 1 && limbs_.back() == 0) {
        limbs_.pop_back();
    }
}

Natural& Natural::operator*=(const Natural& other) {
    std::vector<std::uint32_t> result(
        limbs_.size() + other.limbs_.size(),
        0);
    for (std::size_t left = 0; left < limbs_.size(); ++left) {
        std::uint64_t carry = 0;
        for (std::size_t right = 0; right < other.limbs_.size(); ++right) {
            const auto index = left + right;
            const auto value =
                static_cast<std::uint64_t>(limbs_[left]) *
                    other.limbs_[right] +
                result[index] +
                carry;
            result[index] = static_cast<std::uint32_t>(value);
            carry = value >> 32U;
        }
        result[left + other.limbs_.size()] =
            static_cast<std::uint32_t>(carry);
    }
    limbs_ = std::move(result);
    normalize();
    return *this;
}

std::strong_ordering operator<=>(
    const Natural& left,
    const Natural& right) noexcept {
    if (left.limbs_.size() != right.limbs_.size()) {
        return left.limbs_.size() <=> right.limbs_.size();
    }
    for (auto index = left.limbs_.size(); index-- > 0;) {
        if (left.limbs_[index] != right.limbs_[index]) {
            return left.limbs_[index] <=> right.limbs_[index];
        }
    }
    return std::strong_ordering::equal;
}

std::uint32_t Natural::divide(std::uint32_t divisor) {
    std::uint64_t remainder = 0;
    for (auto index = limbs_.size(); index-- > 0;) {
        const auto value = (remainder << 32U) | limbs_[index];
        limbs_[index] = static_cast<std::uint32_t>(value / divisor);
        remainder = value % divisor;
    }
    normalize();
    return static_cast<std::uint32_t>(remainder);
}

std::string Natural::str() const {
    if (bits() == 0) {
        return "0";
    }
    Natural copy = *this;
    std::vector<std::uint32_t> chunks;
    while (copy.bits() != 0) {
        chunks.push_back(copy.divide(1'000'000'000U));
    }

    std::ostringstream output;
    output << chunks.back();
    for (auto index = chunks.size() - 1; index-- > 0;) {
        output << std::setw(9) << std::setfill('0') << chunks[index];
    }
    return output.str();
}

std::expected<Natural, std::string> power(
    Natural base,
    std::uint64_t exponent,
    std::size_t limit) {
    if (limit == 0) {
        return std::unexpected("natural product exceeds bit limit");
    }
    Natural result(1);
    while (exponent != 0) {
        if ((exponent & 1U) != 0) {
            if (exceeds(result.bits(), base.bits(), limit)) {
                return std::unexpected("natural product exceeds bit limit");
            }
            result *= base;
            if (result.bits() > limit) {
                return std::unexpected("natural product exceeds bit limit");
            }
        }
        exponent >>= 1U;
        if (exponent != 0) {
            if (exceeds(base.bits(), base.bits(), limit)) {
                return std::unexpected("natural product exceeds bit limit");
            }
            base *= base;
            if (base.bits() > limit) {
                return std::unexpected("natural product exceeds bit limit");
            }
        }
    }
    return result;
}

} // namespace mq::kernel::math
