#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"
#include "mq/kernel/gesture/Frame.hpp"
#include "mq/kernel/gesture/Span.hpp"
#include "mq/kernel/performance/Event.hpp"
#include "mq/kernel/phrase/Span.hpp"
#include "mq/kernel/sayr/Completion.hpp"
#include "mq/kernel/tonicization/Level.hpp"
#include "mq/kernel/trace/Event.hpp"

#include <map>
#include <optional>
#include <set>
#include <vector>
#include "mq/kernel/sort/CellId.hpp"
#include "mq/kernel/sort/CenterId.hpp"
#include "mq/kernel/sort/EventId.hpp"
#include "mq/kernel/sort/JinsId.hpp"
#include "mq/kernel/sort/PathId.hpp"
#include "mq/kernel/sort/ObligationId.hpp"
#include "mq/kernel/sort/ProductionId.hpp"

namespace mq::kernel::state {

struct Center {
    std::vector<sort::CenterId> stack;

    bool operator==(const Center&) const = default;
};

struct Jins {
    std::optional<sort::JinsId> active;

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
    std::map<sort::CellId, std::size_t> occurrences;
    std::map<sort::EventId, sort::CellId> owners;

    bool operator==(const Cell&) const = default;
};

struct Trace {
    std::vector<trace::Event> events;

    bool operator==(const Trace&) const = default;
};

struct Path {
    std::set<sort::PathId> completed;

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

struct Gesture {
    std::optional<gesture::Frame> active;
    std::vector<gesture::Span> completed;

    bool operator==(const Gesture&) const = default;
};

struct Sayr {
    std::set<sort::ObligationId> completed;
    std::vector<sayr::Completion> history;

    bool operator==(const Sayr&) const = default;
};

struct Grammar {
    std::map<sort::ProductionId, std::size_t> budget;

    bool operator==(const Grammar&) const = default;
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
    Gesture gesture;
    Sayr sayr;
    Trace trace;
    Grammar grammar;

    bool operator==(const Snapshot&) const = default;
};

} // namespace mq::kernel::state
