#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

namespace mq::kernel::math {

class Natural {
public:
    explicit Natural(std::uint64_t value = 0);

    [[nodiscard]] std::size_t bits() const noexcept;
    [[nodiscard]] std::string str() const;

    Natural& operator*=(const Natural& other);

    friend Natural operator*(Natural left, const Natural& right) {
        return left *= right;
    }

    friend bool operator==(const Natural&, const Natural&) = default;
    friend std::strong_ordering operator<=>(
        const Natural& left,
        const Natural& right) noexcept;

private:
    std::vector<std::uint32_t> limbs_;

    void normalize();
    std::uint32_t divide(std::uint32_t divisor);
};

[[nodiscard]] std::expected<Natural, std::string> power(
    Natural base,
    std::uint64_t exponent,
    std::size_t limit);

} // namespace mq::kernel::math
