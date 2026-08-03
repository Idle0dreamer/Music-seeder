#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/evidence/Kind.hpp"
#include "mq/kernel/motion/Direction.hpp"
#include "mq/kernel/operation/gesture/Begin.hpp"
#include "mq/kernel/operation/gesture/End.hpp"
#include "mq/kernel/operation/sayr/Fulfill.hpp"
#include "mq/kernel/phrase/Boundary.hpp"
#include "mq/kernel/phrase/Function.hpp"
#include "mq/kernel/tonicization/Level.hpp"

#include <optional>
#include <variant>
#include <utility>
#include "mq/kernel/sort/BaggageId.hpp"
#include "mq/kernel/sort/CellId.hpp"
#include "mq/kernel/sort/CenterId.hpp"
#include "mq/kernel/sort/EventId.hpp"
#include "mq/kernel/sort/FamilyId.hpp"
#include "mq/kernel/sort/FormulaId.hpp"
#include "mq/kernel/sort/JinsId.hpp"
#include "mq/kernel/sort/PathId.hpp"
#include "mq/kernel/sort/PhraseId.hpp"
#include "mq/kernel/sort/RegionId.hpp"
#include "mq/kernel/sort/RoleId.hpp"

namespace mq::kernel::operation {

struct Anchor {
    sort::CenterId center;
};

struct Enter {
    sort::JinsId jins;
};

struct Emphasize {
    sort::RoleId role;
    Rational amount{1};
};

struct Dwell {
    sort::RoleId role;
    Rational amount{1};
};

struct Emit {
    sort::CellId cell;
    std::optional<sort::FormulaId> formula;
    std::optional<sort::FormulaId> variation;

    Emit(
        sort::CellId emitted_cell,
        std::optional<sort::FormulaId> emitted_formula,
        std::optional<sort::FormulaId> emitted_variation = std::nullopt)
        : cell(std::move(emitted_cell)),
          formula(std::move(emitted_formula)),
          variation(std::move(emitted_variation)) {}
};

struct Cadence {
    sort::FamilyId family;
    Rational evidence{1};
    Rational strength{1};
};

struct Tonicize {
    sort::JinsId jins;
    tonicization::Level level{tonicization::Level::Color};
};

struct Modulate {
    sort::PathId path;
    sort::CenterId center;
    tonicization::Level level{tonicization::Level::Internal};
};

struct Return {
    sort::CenterId center;
};

struct Place {
    sort::EventId event;
    sort::RoleId role;
    motion::Direction direction{motion::Direction::Start};
    sort::RegionId region;
    std::optional<sort::BaggageId> baggage;
};

struct Begin {
    sort::PhraseId phrase;
    phrase::Function function;
};

struct End {
    sort::PhraseId phrase;
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
    End,
    gesture::Begin,
    gesture::End,
    sayr::Fulfill>;

[[nodiscard]] const char* name(const Any& value) noexcept;

} // namespace mq::kernel::operation
