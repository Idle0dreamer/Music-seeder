#pragma once

#include "mq/kernel/maqam/Package.hpp"

#include <expected>
#include <string>
#include <vector>

namespace mq::kernel::maqam::family {

struct BranchSpec {
    std::string name;
    pitch::Expression target;
    std::string provenance;
};

struct Spec {
    std::string package;
    std::string family;
    pitch::Expression ghammaz;
    pitch::Expression extension;
    std::string provenance;
    std::vector<BranchSpec> branches;
};

[[nodiscard]] std::expected<Scaffold, std::string> make(const Spec&);

} // namespace mq::kernel::maqam::family
