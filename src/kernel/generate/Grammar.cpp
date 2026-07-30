#include "Internal.hpp"

#include "mq/kernel/grammar/Evaluator.hpp"

#include <set>

namespace mq::kernel::generate {
namespace {

std::expected<std::vector<Candidate>, Error> compile(
    const grammar::Result& derivation) {
    std::vector<Candidate> result;
    std::set<Identity> identities;
    result.reserve(derivation.outcomes.size());
    for (const auto& outcome : derivation.outcomes) {
        if (!outcome.candidate ||
            !identities.insert(*outcome.candidate).second ||
            outcome.stages.empty()) {
            return std::unexpected(Error{
                Error::Code::Input,
                "grammar outcomes require unique sealed candidates and stages",
                {},
                derivation.diagnostics,
            });
        }
        std::size_t actions{};
        std::vector<Stage> stages;
        stages.reserve(outcome.stages.size());
        for (const auto& stage : outcome.stages) {
            actions += stage.actions.size();
            stages.push_back({stage.identity, stage.actions});
        }
        if (actions != outcome.program.size()) {
            return std::unexpected(Error{
                Error::Code::Input,
                "grammar candidate has actions outside event stages",
                {},
                derivation.diagnostics,
            });
        }
        result.push_back({
            *outcome.candidate,
            outcome.cost,
            std::move(stages),
        });
    }
    return result;
}

} // namespace

std::expected<Result, Error> Engine::run(
    std::uint64_t seed,
    const Identity& expression,
    const grammar::Term& production,
    const pitch::field::project::Plan& projection,
    const pitch::field::Schema& schema,
    state::Snapshot initial,
    Limits limits) const {
    const auto derivation =
        paths_ == nullptr
            ? grammar::Evaluator(profile_).derive(production, initial)
            : grammar::Evaluator(profile_, *paths_).derive(
                  production,
                  initial);
    if (derivation.outcomes.empty()) {
        return std::unexpected(Error{
            Error::Code::NoLegal,
            "grammar has no legal complete candidate",
            {},
            derivation.diagnostics,
        });
    }
    const auto candidates = compile(derivation);
    if (!candidates) {
        return std::unexpected(candidates.error());
    }
    auto generated = run(
        seed,
        expression,
        *candidates,
        projection,
        schema,
        std::move(initial),
        limits);
    if (!generated) {
        auto error = std::move(generated.error());
        error.derivation = derivation.diagnostics;
        return std::unexpected(std::move(error));
    }
    generated->derivation = derivation.diagnostics;
    return generated;
}

} // namespace mq::kernel::generate
