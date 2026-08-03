#pragma once

#include "mq/kernel/performance/Event.hpp"
#include "mq/kernel/pitch/Expression.hpp"
#include "mq/kernel/sort/CellId.hpp"
#include "mq/kernel/sort/FormulaId.hpp"
#include "mq/kernel/sort/MotifId.hpp"

#include <optional>

namespace mq::kernel::performance {

struct Target {
    Event event;
    pitch::Expression center;
    // Structural ownership is optional only for neutral test fixtures. A
    // collection-backed generated event carries these authorities through
    // the performance boundary so renderers and reports can inspect the
    // derivation that produced the sound.
    std::optional<sort::CellId> cell;
    std::optional<sort::FormulaId> formula;
    std::optional<sort::FormulaId> variation;
    std::optional<sort::MotifId> motif;

    bool operator==(const Target&) const = default;
};

} // namespace mq::kernel::performance
