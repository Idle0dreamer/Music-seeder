#pragma once

#include "mq/kernel/Rational.hpp"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace mq::kernel::pitch::order {

enum class Relation {
    Less,
    Equal,
    Greater,
};

struct Limits {
    std::uint64_t exponent{1'000'000};
    std::size_t product{1'000'000};
};

struct Error {
    enum class Code {
        Scale,
        Exponent,
        Product,
        Prime,
    };

    Code code;
    std::string message;
};

struct Proof {
    Relation relation;
    std::map<std::int64_t, Rational> normalized;
    std::uint64_t scale{1};
    std::string positive{"1"};
    std::string negative{"1"};

    bool operator==(const Proof&) const = default;
};

} // namespace mq::kernel::pitch::order
