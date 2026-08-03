#include "Internal.hpp"

namespace mq::kernel::pitch::request {

std::expected<Result, Error> run(
    const state::Snapshot& state,
    const field::project::Plan& projection,
    const field::Schema& schema,
    performance::Plan prefix,
    Limits limits) {
    const auto current =
        detail::check(state, prefix, limits.history);
    if (!current) {
        return std::unexpected(current.error());
    }

    auto context = field::project::run(
        projection,
        state,
        limits.projection);
    if (!context) {
        return std::unexpected(Error{
            Error::Code::Project,
            context.error().message,
            context.error(),
            std::nullopt,
            std::nullopt,
        });
    }
    auto solved = field::solve::run(
        schema,
        *context,
        limits.field);
    if (!solved) {
        return std::unexpected(Error{
            Error::Code::Field,
            solved.error().message,
            std::nullopt,
            solved.error(),
            std::nullopt,
        });
    }

    const auto center = solved->solution.values.find((*current)->role);
    if (center == solved->solution.values.end()) {
        return std::unexpected(Error{
            Error::Code::Role,
            "calculated field has no value for current structural role",
            std::nullopt,
            std::nullopt,
            std::nullopt,
        });
    }
    auto proof = detail::prove(
        **current,
        center->second,
        prefix,
        limits.order);
    if (!proof) {
        return std::unexpected(proof.error());
    }

    if (!limits.timing || !limits.timing->well_formed()) {
        return std::unexpected(Error{
            Error::Code::Plan,
            "timing policy is not well formed",
            std::nullopt,
            std::nullopt,
            std::nullopt,
        });
    }
    const auto index = state.melody.history.size() - 1;
    const bool phrase_start =
        state.phrase.active && state.phrase.active->first == index;
    const bool phrase_end =
        !state.phrase.active && !state.phrase.completed.empty() &&
        state.phrase.completed.back().last == (*current)->identity;
    const bool cadence =
        phrase_end && !state.phrase.completed.empty() &&
        state.phrase.completed.back().last == (*current)->identity;
    const performance::Context timing_context{
        index,
        phrase_start,
        phrase_end,
        cadence,
        limits.seed,
    };
    const auto timing = limits.timing->resolve(
        (*current)->direction,
        timing_context);
    const auto owner = state.cell.owners.find(
        sort::EventId{(*current)->identity});
    const bool formula_variation =
        owner != state.cell.owners.end() && owner->second.variation.has_value();

    const auto previous = prefix.events.empty()
                              ? std::optional<pitch::Expression>{}
                              : std::optional<pitch::Expression>{
                                    prefix.events.back().target.center};
    std::optional<performance::Release> release;
    if (timing.release_duration > Rational(0)) {
        release = performance::Release{
            timing.release_duration,
            timing.release_articulation,
        };
    }
    prefix.append(
        performance::Target{
            **current,
            center->second,
            owner == state.cell.owners.end()
                ? std::nullopt
                : std::optional<sort::CellId>{owner->second.cell},
            owner == state.cell.owners.end()
                ? std::nullopt
                : owner->second.formula,
            owner == state.cell.owners.end()
                ? std::nullopt
                : owner->second.variation,
            owner == state.cell.owners.end()
                ? std::nullopt
                : owner->second.motif,
            owner == state.cell.owners.end()
                ? std::nullopt
                : owner->second.transformation,
            owner == state.cell.owners.end()
                ? std::string{}
                : owner->second.transformation_provenance,
        },
        timing.duration,
        timing.intensity,
        timing.articulation,
        performance::monophonic(),
        release);
    const auto pause = limits.timing->pause(timing_context);
    if (pause > Rational(0)) {
        prefix.append_pause(
            pause,
            Identity{
                "performance.pause",
                cadence ? "cadence" : "phrase-boundary",
                "1",
            },
            limits.timing->provenance + ";derived:phrase-boundary");
    }
    if (previous &&
        ((*current)->direction == motion::Direction::Rise ||
         (*current)->direction == motion::Direction::Fall)) {
        if (formula_variation) {
            prefix.events.back().ornament = performance::Ornament{
                owner->second.variation->identity,
                performance::OrnamentKind::Oscillation,
                (*previous - center->second) * timing.ornament_timing.duration,
                Rational(1),
                timing.ornament_timing,
                "derived:collection-formula-variation;timing-profile",
            };
        } else {
            prefix.events.back().contour = performance::PitchContour{
                {
                    {Rational(0), *previous - center->second},
                    {Rational(1), pitch::Expression{}},
                },
            };
            prefix.events.back().ornament = performance::Ornament{
                Identity{
                    "performance.ornament",
                    "directional-approach",
                    "1",
                },
                performance::OrnamentKind::Approach,
                *previous - center->second,
                Rational(1),
                timing.ornament_timing,
                "derived:structural-motion;profile-may-refine",
            };
        }
    }
    return Result{
        std::move(prefix),
        std::move(*context),
        std::move(*solved),
        std::move(*proof),
    };
}

} // namespace mq::kernel::pitch::request
