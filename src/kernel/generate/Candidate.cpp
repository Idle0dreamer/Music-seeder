#include "Internal.hpp"

#include <algorithm>
#include <set>

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
    eval::Context context,
    const pitch::field::project::Plan& projection,
    const pitch::field::Schema& schema,
    state::Snapshot state,
    Limits limits) {
    if (!limits.timing) {
        return std::unexpected(Diagnostic{
            candidate.identity,
            std::nullopt,
            "generation requires an explicit performance timing profile",
            std::nullopt,
            std::nullopt,
        });
    }
    limits.pitch.timing = *limits.timing;
    limits.pitch.seed = limits.seed;
    performance::Plan plan;
    for (const auto& stage : candidate.stages) {
        const auto before = state.melody.history.size();
        const auto next = eval::Evaluator(
            profile,
            context).run(state, stage.actions);
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
    if (state.gesture.active) {
        return std::unexpected(Diagnostic{
            candidate.identity,
            candidate.stages.back().identity,
            "candidate ended with an active gesture",
            std::nullopt,
            std::nullopt,
        });
    }
    const bool every_event_has_cell =
        state.cell.owners.size() == state.melody.history.size() &&
        std::ranges::all_of(
            state.melody.history,
            [&](const auto& event) {
                return state.cell.owners.contains(
                    sort::EventId{event.identity});
            });
    if (!every_event_has_cell) {
        return std::unexpected(Diagnostic{
            candidate.identity,
            candidate.stages.back().identity,
            "candidate contains a structural event without a cell owner",
            std::nullopt,
            std::nullopt,
        });
    }
    if (context.sayr.plan != nullptr) {
        std::set<sort::ObligationId> history;
        for (const auto& completion : state.sayr.history) {
            history.insert(completion.obligation);
        }
        const bool consistent =
            history.size() == state.sayr.history.size() &&
            history == state.sayr.completed;
        if (!consistent ||
            !context.sayr.plan->accepts(state.sayr.history)) {
            const auto rule =
                consistent ? "sayr.route" : "sayr.state";
            const auto message =
                consistent
                    ? "candidate did not complete a declared sayr route"
                    : "candidate has inconsistent sayr completion state";
            return std::unexpected(Diagnostic{
                candidate.identity,
                candidate.stages.back().identity,
                message,
                eval::Violation{
                    state.trace.events.size(),
                    "Candidate",
                    rule,
                    message,
                },
                std::nullopt,
            });
        }
    }
    return Outcome{
        candidate.identity,
        candidate.cost,
        std::move(state),
        std::move(plan),
    };
}

} // namespace mq::kernel::generate::detail
