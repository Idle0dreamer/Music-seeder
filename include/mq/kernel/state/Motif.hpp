#pragma once

#include "mq/kernel/sort/EventId.hpp"
#include "mq/kernel/sort/FormulaId.hpp"
#include "mq/kernel/sort/MotifId.hpp"

#include <map>
#include <optional>
#include <vector>

namespace mq::kernel::state {

struct MotifOccurrence {
    sort::EventId event;
    std::optional<sort::FormulaId> formula;
    std::optional<sort::FormulaId> variation;

    bool operator==(const MotifOccurrence&) const = default;
};

struct Motif {
    std::map<sort::MotifId, std::vector<MotifOccurrence>> occurrences;

    bool operator==(const Motif&) const = default;
};

} // namespace mq::kernel::state
