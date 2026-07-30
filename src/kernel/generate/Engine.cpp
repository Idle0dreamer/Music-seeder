#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::generate {

Engine::Engine(const profile::Set& profile)
    : Engine(profile, eval::Context{}) {}

Engine::Engine(
    const profile::Set& profile,
    eval::Context context)
    : profile_(profile),
      context_(context) {}

Engine::Engine(
    const profile::Set& profile,
    const path::Graph& paths)
    : Engine(
          profile,
          eval::Context{
              .jins = {},
              .path = {&paths},
              .sayr = {},
          }) {}

Engine::Engine(
    const profile::Set& profile,
    const jins::Catalog& catalog)
    : Engine(
          profile,
          eval::Context{
              .jins = {&catalog},
              .path = {},
              .sayr = {},
          }) {}

Engine::Engine(
    const profile::Set& profile,
    const jins::Catalog& catalog,
    const path::Graph& paths)
    : Engine(
          profile,
          eval::Context{
              .jins = {&catalog},
              .path = {&paths},
              .sayr = {},
          }) {}

std::expected<Result, Error> Engine::run(
    std::uint64_t seed,
    const Identity& expression,
    std::span<const Candidate> candidates,
    const pitch::field::project::Plan& projection,
    const pitch::field::Schema& schema,
    state::Snapshot initial,
    Limits limits) const {
    const auto valid = detail::check(expression, candidates, limits);
    if (!valid) {
        return std::unexpected(valid.error());
    }

    std::vector<Outcome> legal;
    std::vector<Diagnostic> rejected;
    std::vector<choice::Candidate> choices;
    legal.reserve(candidates.size());
    rejected.reserve(candidates.size());
    choices.reserve(candidates.size());
    for (const auto& candidate : candidates) {
        auto outcome = detail::evaluate(
            candidate,
            profile_,
            context_,
            projection,
            schema,
            initial,
            limits);
        if (!outcome) {
            rejected.push_back(std::move(outcome.error()));
            continue;
        }
        choices.push_back({
            candidate.identity,
            candidate.cost,
            true,
        });
        legal.push_back(std::move(*outcome));
    }
    if (legal.empty()) {
        return std::unexpected(Error{
            Error::Code::NoLegal,
            "generation has no legal complete candidate",
            std::move(rejected),
            {},
        });
    }
    const auto selected =
        choice::select(seed, expression, {}, choices);
    if (!selected) {
        return std::unexpected(Error{
            Error::Code::Choice,
            selected.error(),
            std::move(rejected),
            {},
        });
    }
    return Result{
        *selected,
        std::move(legal),
        std::move(rejected),
        {},
    };
}

} // namespace mq::kernel::generate
