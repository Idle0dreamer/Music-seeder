#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"
#include "mq/kernel/performance/Event.hpp"
#include "mq/kernel/phrase/Span.hpp"
#include "mq/kernel/tonicization/Level.hpp"
#include "mq/kernel/trace/Event.hpp"

#include <map>
#include <optional>
#include <set>
#include <vector>

namespace mq::kernel::state {

struct Center {
    std::vector<Identity> stack;

    bool operator==(const Center&) const = default;
};

struct Jins {
    std::optional<Identity> active;

    bool operator==(const Jins&) const = default;
};

struct Tonicization {
    tonicization::Level level{tonicization::Level::Color};

    bool operator==(const Tonicization&) const = default;
};

struct Evidence {
    std::map<evidence::Kind, Rational> amount;

    bool operator==(const Evidence&) const = default;
};

struct Cell {
    std::map<Identity, std::size_t> occurrences;

    bool operator==(const Cell&) const = default;
};

struct Trace {
    std::vector<trace::Event> events;

    bool operator==(const Trace&) const = default;
};

struct Path {
    std::set<Identity> completed;

    bool operator==(const Path&) const = default;
};

struct Melody {
    std::optional<performance::Event> current;
    std::vector<performance::Event> history;

    bool operator==(const Melody&) const = default;
};

struct Phrase {
    std::optional<phrase::Frame> active;
    std::vector<phrase::Span> completed;

    bool operator==(const Phrase&) const = default;
};

struct Snapshot {
    Center center;
    Jins jins;
    Tonicization tonicization;
    Evidence evidence;
    Cell cell;
    Path path;
    Melody melody;
    Phrase phrase;
    Trace trace;

    bool operator==(const Snapshot&) const = default;
};

} // namespace mq::kernel::state
