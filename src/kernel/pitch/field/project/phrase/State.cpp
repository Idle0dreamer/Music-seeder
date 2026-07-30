#include "Internal.hpp"

namespace mq::kernel::pitch::field::project::detail::phrase {

std::expected<const performance::Event*, Error> current(
    const state::Snapshot& state) {
    if (!state.melody.current) {
        return static_cast<const performance::Event*>(nullptr);
    }
    if (state.melody.history.empty() ||
        state.melody.history.back() != *state.melody.current) {
        return std::unexpected(Error{
            Error::Code::Input,
            "current event and event history disagree",
            std::nullopt,
        });
    }
    return &*state.melody.current;
}

const mq::kernel::phrase::Frame* active(
    const state::Snapshot& state) {
    if (!state.phrase.active ||
        state.melody.history.size() <= state.phrase.active->first) {
        return nullptr;
    }
    return &*state.phrase.active;
}

std::expected<const mq::kernel::phrase::Span*, Error> completed(
    const state::Snapshot& state) {
    const auto event = current(state);
    if (!event) {
        return std::unexpected(event.error());
    }
    if (*event == nullptr) {
        return static_cast<const mq::kernel::phrase::Span*>(nullptr);
    }

    const mq::kernel::phrase::Span* match = nullptr;
    for (const auto& span : state.phrase.completed) {
        if (span.last != (*event)->identity) {
            continue;
        }
        if (match != nullptr) {
            return std::unexpected(Error{
                Error::Code::Input,
                "multiple completed phrases end at the current event",
                std::nullopt,
            });
        }
        match = &span;
    }
    return match;
}

std::expected<std::optional<Fact>, Error> absent(
    const Identity& key,
    Presence presence) {
    if (presence == Presence::Required) {
        return std::unexpected(Error{
            Error::Code::Missing,
            "required phrase projection state is missing",
            key,
        });
    }
    return std::optional<Fact>{};
}

} // namespace mq::kernel::pitch::field::project::detail::phrase
