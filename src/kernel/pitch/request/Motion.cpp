#include "Internal.hpp"

namespace mq::kernel::pitch::request::detail {
namespace {

order::Relation expected(motion::Direction direction) {
    switch (direction) {
    case motion::Direction::Same:
        return order::Relation::Equal;
    case motion::Direction::Rise:
        return order::Relation::Greater;
    case motion::Direction::Fall:
        return order::Relation::Less;
    case motion::Direction::Start:
        break;
    }
    return order::Relation::Equal;
}

Error failure(
    Error::Code code,
    std::string message,
    std::optional<order::Error> cause = {}) {
    return {
        code,
        std::move(message),
        std::nullopt,
        std::nullopt,
        std::move(cause),
    };
}

} // namespace

std::expected<std::optional<order::Proof>, Error> prove(
    const performance::Event& event,
    const Expression& center,
    const performance::Plan& prefix,
    order::Limits limits) {
    if (prefix.targets.empty()) {
        if (event.direction != motion::Direction::Start) {
            return std::unexpected(failure(
                Error::Code::Direction,
                "first performance target is not Start"));
        }
        return std::optional<order::Proof>{};
    }
    if (event.direction == motion::Direction::Start) {
        return std::unexpected(failure(
            Error::Code::Direction,
            "continuing performance target declares Start"));
    }

    auto proof = order::compare(
        center,
        prefix.targets.back().center,
        limits);
    if (!proof) {
        return std::unexpected(failure(
            Error::Code::Order,
            proof.error().message,
            proof.error()));
    }
    if (proof->relation != expected(event.direction)) {
        return std::unexpected(failure(
            Error::Code::Direction,
            "calculated target contradicts intended " +
                std::string(motion::name(event.direction))));
    }
    return std::optional<order::Proof>{std::move(*proof)};
}

} // namespace mq::kernel::pitch::request::detail
