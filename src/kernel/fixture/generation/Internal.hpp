#pragma once

#include "mq/kernel/fixture/generation/Set.hpp"

namespace mq::kernel::fixture::generation::detail {

[[nodiscard]] Identity id(std::string name);

[[nodiscard]] generate::Stage stage(
    const fixture::Set& fixture,
    std::string name,
    bool travel);

[[nodiscard]] grammar::Term actions(
    const generate::Stage& stage,
    const std::string& prefix);

[[nodiscard]] grammar::Term candidate(
    const generate::Candidate& candidate,
    const std::string& prefix);

} // namespace mq::kernel::fixture::generation::detail
