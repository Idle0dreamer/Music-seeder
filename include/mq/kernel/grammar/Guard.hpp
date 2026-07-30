#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"
#include "mq/kernel/state/Snapshot.hpp"

#include <set>
#include <variant>

namespace mq::kernel::grammar::guard {

struct Bound {
    Identity binding;
};

struct Center {
    Identity identity;
};

struct Jins {
    Identity identity;
};

struct Evidence {
    evidence::Kind kind;
    Rational minimum;
};

using Predicate = std::variant<Bound, Center, Jins, Evidence>;

[[nodiscard]] bool holds(
    const Predicate& predicate,
    const state::Snapshot& state,
    const std::set<Identity>& bindings);

} // namespace mq::kernel::grammar::guard
