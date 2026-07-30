#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"
#include "mq/kernel/motion/Direction.hpp"
#include "mq/kernel/phrase/Boundary.hpp"
#include "mq/kernel/phrase/Function.hpp"
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
    Rational strength{1};
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

struct Place {
    Identity event;
    Identity role;
    motion::Direction direction{motion::Direction::Start};
};

struct Begin {
    Identity phrase;
    phrase::Function function;
};

struct End {
    Identity phrase;
    phrase::Boundary boundary{phrase::Boundary::Open};
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
    Return,
    Place,
    Begin,
    End>;

[[nodiscard]] const char* name(const Any& value) noexcept;

} // namespace mq::kernel::operation
