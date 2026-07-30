#pragma once

#include "mq/kernel/grammar/Result.hpp"
#include "mq/kernel/grammar/Term.hpp"
#include "mq/kernel/profile/Set.hpp"

#include <cstdint>
#include <expected>
#include <string>

namespace mq::kernel::grammar {

class Evaluator {
public:
    explicit Evaluator(const profile::Set& profile);
    Evaluator(const profile::Set& profile, const path::Graph& paths);

    [[nodiscard]] Result derive(
        const Term& term,
        state::Snapshot state = {}) const;

    [[nodiscard]] std::expected<Outcome, std::string> choose(
        std::uint64_t seed,
        const Term& term,
        state::Snapshot state = {}) const;

private:
    const profile::Set& profile_;
    const path::Graph* paths_{};
};

} // namespace mq::kernel::grammar
