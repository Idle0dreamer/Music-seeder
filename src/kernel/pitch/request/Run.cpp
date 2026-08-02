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

    const auto previous = prefix.events.empty()
                              ? std::optional<pitch::Expression>{}
                              : std::optional<pitch::Expression>{
                                    prefix.events.back().target.center};
    prefix.append(performance::Target{
        **current,
        center->second,
    });
    if (previous &&
        ((*current)->direction == motion::Direction::Rise ||
         (*current)->direction == motion::Direction::Fall)) {
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
            "derived:structural-motion;profile-may-refine",
        };
    }
    return Result{
        std::move(prefix),
        std::move(*context),
        std::move(*solved),
        std::move(*proof),
    };
}

} // namespace mq::kernel::pitch::request
