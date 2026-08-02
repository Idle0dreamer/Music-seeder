#pragma once

#include "mq/kernel/maqam/Package.hpp"
#include "mq/kernel/motion/Direction.hpp"

#include <expected>
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

struct Spec {
    std::string package;
    std::string family;
    pitch::Expression ghammaz;
    pitch::Expression extension;
    std::string upper_role;
    std::string provenance;
    std::vector<BranchSpec> branches;
    std::vector<std::string> root_roles;
    bool ordered{};
};

[[nodiscard]] std::expected<Scaffold, std::string> make(const Spec&);

} // namespace mq::kernel::maqam::family
