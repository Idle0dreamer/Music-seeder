#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"
#include "mq/kernel/phrase/Boundary.hpp"

#include <optional>
#include <variant>

namespace mq::kernel::sayr::proof {

struct Jins {
    Identity identity;

    bool operator==(const Jins&) const = default;
};

struct Center {
    Identity identity;

    bool operator==(const Center&) const = default;
};

struct Role {
    Identity event;
    Identity role;

    bool operator==(const Role&) const = default;
};

struct Region {
    Identity event;
    Identity region;

    bool operator==(const Region&) const = default;
};

struct Gesture {
    Identity occurrence;
    Identity family;

    bool operator==(const Gesture&) const = default;
};

struct Phrase {
    Identity phrase;
    Identity function;
    mq::kernel::phrase::Boundary boundary;
    std::optional<Identity> cadence;

    bool operator==(const Phrase&) const = default;
};

struct Path {
    Identity identity;

    bool operator==(const Path&) const = default;
};

struct Evidence {
    evidence::Kind kind;
    Rational amount;

    bool operator==(const Evidence&) const = default;
};

using Item = std::variant<
    Jins,
    Center,
    Role,
    Region,
    Gesture,
    Phrase,
    Path,
    Evidence>;

} // namespace mq::kernel::sayr::proof
