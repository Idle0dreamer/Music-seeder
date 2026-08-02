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

    prefix.append(performance::Target{
        **current,
        center->second,
    });
    return Result{
        std::move(prefix),
        std::move(*context),
        std::move(*solved),
        std::move(*proof),
    };
}

} // namespace mq::kernel::pitch::request
