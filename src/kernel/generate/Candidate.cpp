#include "Internal.hpp"

namespace mq::kernel::generate::detail {
namespace {

Diagnostic failed(
    const Candidate& candidate,
    const Stage& stage,
    const eval::Violation& error) {
    return {
        candidate.identity,
        stage.identity,
        error.message,
        error,
        std::nullopt,
    };
}

Diagnostic failed(
    const Candidate& candidate,
    const Stage& stage,
    const pitch::request::Error& error) {
    return {
        candidate.identity,
        stage.identity,
        error.message,
        std::nullopt,
        error,
    };
}

} // namespace

std::expected<Outcome, Diagnostic> evaluate(
    const Candidate& candidate,
    const profile::Set& profile,
    const path::Graph* paths,
    const pitch::field::project::Plan& projection,
    const pitch::field::Schema& schema,
    state::Snapshot state,
    Limits limits) {
    performance::Plan plan;
    for (const auto& stage : candidate.stages) {
        const auto before = state.melody.history.size();
        auto next = paths == nullptr
                        ? eval::Evaluator(profile).run(state, stage.actions)
                        : eval::Evaluator(profile, *paths).run(
                              state,
                              stage.actions);
        if (!next) {
            return std::unexpected(failed(candidate, stage, next.error()));
        }
        if (next->melody.history.size() != before + 1) {
            return std::unexpected(Diagnostic{
                candidate.identity,
                stage.identity,
                "stage did not commit exactly one structural event",
                std::nullopt,
                std::nullopt,
            });
        }
        state = std::move(*next);
        auto target = pitch::request::run(
            state,
            projection,
            schema,
            std::move(plan),
            limits.pitch);
        if (!target) {
            return std::unexpected(
                failed(candidate, stage, target.error()));
        }
        plan = std::move(target->plan);
    }
    if (state.phrase.active) {
        return std::unexpected(Diagnostic{
            candidate.identity,
            candidate.stages.back().identity,
            "candidate ended with an active phrase",
            std::nullopt,
            std::nullopt,
        });
    }
    return Outcome{
        candidate.identity,
        candidate.cost,
        std::move(state),
        std::move(plan),
    };
}

} // namespace mq::kernel::generate::detail
