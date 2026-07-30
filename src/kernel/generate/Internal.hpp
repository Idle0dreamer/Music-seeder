#pragma once

#include "mq/kernel/generate/Engine.hpp"

namespace mq::kernel::generate::detail {

[[nodiscard]] bool complete(const Identity& identity);

[[nodiscard]] std::expected<void, Error> check(
    const Identity& choice,
    std::span<const Candidate> candidates,
    Limits limits);

[[nodiscard]] std::expected<Outcome, Diagnostic> evaluate(
    const Candidate& candidate,
    const profile::Set& profile,
    const path::Graph* paths,
    const pitch::field::project::Plan& projection,
    const pitch::field::Schema& schema,
    state::Snapshot initial,
    Limits limits);

} // namespace mq::kernel::generate::detail
