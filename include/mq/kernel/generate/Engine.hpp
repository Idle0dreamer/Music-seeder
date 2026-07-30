#pragma once

#include "mq/kernel/generate/Candidate.hpp"
#include "mq/kernel/generate/Limits.hpp"
#include "mq/kernel/generate/Result.hpp"
#include "mq/kernel/grammar/Term.hpp"
#include "mq/kernel/path/Graph.hpp"
#include "mq/kernel/pitch/field/Schema.hpp"
#include "mq/kernel/pitch/field/project/Plan.hpp"
#include "mq/kernel/profile/Set.hpp"

#include <cstdint>
#include <expected>
#include <span>

namespace mq::kernel::generate {

class Engine {
public:
    explicit Engine(const profile::Set& profile);
    Engine(const profile::Set& profile, const path::Graph& paths);

    [[nodiscard]] std::expected<Result, Error> run(
        std::uint64_t seed,
        const Identity& choice,
        std::span<const Candidate> candidates,
        const pitch::field::project::Plan& projection,
        const pitch::field::Schema& schema,
        state::Snapshot initial = {},
        Limits limits = {}) const;

    [[nodiscard]] std::expected<Result, Error> run(
        std::uint64_t seed,
        const Identity& choice,
        const grammar::Term& production,
        const pitch::field::project::Plan& projection,
        const pitch::field::Schema& schema,
        state::Snapshot initial = {},
        Limits limits = {}) const;

private:
    const profile::Set& profile_;
    const path::Graph* paths_{};
};

} // namespace mq::kernel::generate
