#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"
#include "mq/kernel/tonicization/Level.hpp"

#include <variant>

namespace mq::kernel::operation {

struct Anchor {
    Identity center;
};

struct Enter {
    Identity jins;
};

struct Emphasize {
    Identity role;
    Rational amount{1};
};

struct Dwell {
    Identity role;
    Rational amount{1};
};

struct Emit {
    Identity cell;
};

struct Cadence {
    Identity family;
    Rational evidence{1};
};

struct Tonicize {
    Identity jins;
    tonicization::Level level{tonicization::Level::Color};
};

struct Modulate {
    Identity path;
    Identity center;
    tonicization::Level level{tonicization::Level::Internal};
};

struct Return {
    Identity center;
};

using Any = std::variant<
    Anchor,
    Enter,
    Emphasize,
    Dwell,
    Emit,
    Cadence,
    Tonicize,
    Modulate,
    Return>;

[[nodiscard]] const char* name(const Any& value) noexcept;

} // namespace mq::kernel::operation
