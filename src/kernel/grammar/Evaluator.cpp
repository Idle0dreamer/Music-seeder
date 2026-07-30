#include "mq/kernel/grammar/Evaluator.hpp"

#include "Run.hpp"

#include <limits>

namespace mq::kernel::grammar {

Evaluator::Evaluator(const profile::Set& profile)
    : Evaluator(profile, eval::Context{}) {}

Evaluator::Evaluator(
    const profile::Set& profile,
    eval::Context context)
    : profile_(profile),
      context_(context) {}

Evaluator::Evaluator(const profile::Set& profile, const path::Graph& paths)
    : Evaluator(
          profile,
          eval::Context{
              .jins = {},
              .path = {&paths},
              .sayr = {},
          }) {}

Evaluator::Evaluator(
    const profile::Set& profile,
    const jins::Catalog& catalog)
    : Evaluator(
          profile,
          eval::Context{
              .jins = {&catalog},
              .path = {},
              .sayr = {},
          }) {}

Evaluator::Evaluator(
    const profile::Set& profile,
    const jins::Catalog& catalog,
    const path::Graph& paths)
    : Evaluator(
          profile,
          eval::Context{
              .jins = {&catalog},
              .path = {&paths},
              .sayr = {},
          }) {}

Result Evaluator::derive(
    const Term& term,
    state::Snapshot state) const {
    detail::Runner runner(profile_, context_);
    auto batch = runner.run(
        term,
        detail::Frame{
            Outcome{
                .candidate = {},
                .state = std::move(state),
                .cost = {},
                .decisions = {},
                .program = {},
                .stages = {},
            },
            {},
            {},
        });

    Result result;
    result.diagnostics = std::move(batch.diagnostics);
    result.outcomes.reserve(batch.frames.size());
    for (auto& frame : batch.frames) {
        result.outcomes.push_back(std::move(frame.outcome));
    }
    return result;
}

std::expected<Outcome, std::string> Evaluator::choose(
    std::uint64_t seed,
    const Term& term,
    state::Snapshot state) const {
    const auto result = derive(term, std::move(state));
    if (result.outcomes.empty()) {
        const auto message = result.diagnostics.empty()
                               ? "grammar has no legal outcome"
                               : result.diagnostics.front().message;
        return std::unexpected(message);
    }

    const Outcome* best = nullptr;
    auto rank = std::numeric_limits<std::uint64_t>::max();
    for (const auto& outcome : result.outcomes) {
        const auto candidate = choice::key(seed, outcome.decisions);
        if (best == nullptr || outcome.cost < best->cost ||
            (outcome.cost == best->cost && candidate < rank)) {
            best = &outcome;
            rank = candidate;
        }
    }
    return *best;
}

} // namespace mq::kernel::grammar
