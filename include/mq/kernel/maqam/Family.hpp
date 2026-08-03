#pragma once

#include "mq/kernel/maqam/Package.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/motion/Direction.hpp"

#include <expected>
#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace mq::kernel::maqam::family {

struct BranchSpec {
    std::string name;
    pitch::Expression target;
    std::string provenance;
    std::string source_center_name;
    std::string center_name;
    std::string tonic_role;
    std::string ghammaz_role;
    std::string descent_role;
    motion::Direction direction{motion::Direction::Fall};

    BranchSpec() = default;

    BranchSpec(
        std::string branch_name,
        pitch::Expression branch_target,
        std::string branch_provenance,
        std::string branch_source_center = {},
        std::string branch_center = {},
        std::string branch_tonic = {},
        std::string branch_ghammaz = {},
        motion::Direction branch_direction = motion::Direction::Fall,
        std::string branch_descent = {})
        : name(std::move(branch_name)),
          target(std::move(branch_target)),
          provenance(std::move(branch_provenance)),
          source_center_name(std::move(branch_source_center)),
          center_name(std::move(branch_center)),
          tonic_role(std::move(branch_tonic)),
          ghammaz_role(std::move(branch_ghammaz)),
          descent_role(std::move(branch_descent)),
          direction(branch_direction) {}
};

struct ActionSpec {
    std::string operation;
    std::vector<std::string> arguments;
    std::optional<std::size_t> variant;
};

struct StepSpec {
    std::string name;
    std::string branch;
    std::vector<ActionSpec> actions;
    std::optional<std::vector<std::string>> next;
    std::string transition_provenance;
    std::size_t minimum{1};
    std::size_t maximum{1};
};

struct RouteSpec {
    std::string name;
    std::vector<std::string> branches;
    std::vector<std::string> terminals;
    std::size_t variants{1};
    std::vector<StepSpec> steps;
};

struct AuthoritySpec {
    std::string kind;
    std::vector<std::string> names;
};

struct JinsSpec {
    std::string name;
    std::string tonic;
    std::vector<std::string> roles;
    std::vector<std::string> ghammaz;
    std::vector<std::string> regions;
    std::vector<std::string> characteristic;
    std::vector<std::string> emphasis;
    std::vector<std::string> entry;
    std::vector<std::string> exit;
    std::vector<std::string> cadences;
    std::vector<std::string> motifs;
};

struct GestureStepSpec {
    std::vector<std::string> roles;
    std::vector<std::string> regions;
    motion::Direction direction{motion::Direction::Start};
    std::optional<std::string> baggage;
};

struct GestureSpec {
    std::string owner;
    std::string name;
    std::vector<GestureStepSpec> steps;
};

struct BaggageSpec {
    std::string owner;
    std::string name;
    std::string role;
    std::vector<std::string> regions;
    std::vector<motion::Direction> directions;
    std::vector<std::string> gestures;
};

struct NeedSpec {
    std::string kind;
    std::vector<std::string> arguments;
};

struct ObligationSpec {
    std::string name;
    std::vector<std::string> after;
    std::vector<NeedSpec> needs;
};

struct FormulaNoteSpec {
    std::string event;
    std::string role;
    std::string direction;
    std::string region;
    std::optional<std::string> baggage;
    Rational emphasis{};
    Rational dwell{};
};

struct FormulaSpec {
    std::string name;
    std::string cell;
    std::string provenance;
    std::vector<FormulaNoteSpec> notes;
};

struct FormulaVariationSpec {
    std::string base;
    std::string variation;
    std::string transformation;
    std::string provenance;
};

struct Spec {
    std::string package;
    std::string family;
    pitch::Expression ghammaz;
    pitch::Expression extension;
    std::string upper_role;
    std::string provenance;
    std::vector<BranchSpec> branches;
    std::vector<std::string> root_roles;
    std::vector<AuthoritySpec> authorities;
    std::vector<JinsSpec> jins;
    std::vector<GestureSpec> gestures;
    std::vector<BaggageSpec> baggage;
    std::vector<ObligationSpec> obligations;
    std::vector<FormulaSpec> formulas;
    std::vector<FormulaVariationSpec> formula_variations;
    std::vector<RouteSpec> routes;
};

[[nodiscard]] std::expected<Scaffold, std::string> make(const Spec&);

} // namespace mq::kernel::maqam::family
