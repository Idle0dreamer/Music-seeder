#pragma once

#include "mq/kernel/fixture/generation/Set.hpp"

namespace mq::kernel::fixture::generation::detail {

struct Key {
    Identity role;
    Identity motion;
    Identity region;
    Identity baggage;
    Identity gesture;
    Identity function;
    Identity cadence;
    Identity boundary;
    Identity start;
    Identity same;
    Identity rise;
    Identity fall;
    Identity open;
    Identity closed;
};

[[nodiscard]] Identity id(std::string name);

[[nodiscard]] Key keys();

[[nodiscard]] std::vector<generate::Stage> stages(
    const fixture::Set& fixture,
    std::string name,
    bool journey);

[[nodiscard]] generate::Stage stay(
    const fixture::Set& fixture,
    const std::string& name);

[[nodiscard]] std::vector<generate::Stage> travel(
    const fixture::Set& fixture,
    const std::string& name);

[[nodiscard]] grammar::Term actions(
    const generate::Stage& stage,
    const std::string& prefix);

[[nodiscard]] grammar::Term candidate(
    const generate::Candidate& candidate,
    const std::string& prefix);

[[nodiscard]] pitch::field::project::Plan projection(
    const Key& key);

[[nodiscard]] pitch::field::Schema schema(
    const fixture::Set& fixture,
    const Key& key);

} // namespace mq::kernel::fixture::generation::detail
