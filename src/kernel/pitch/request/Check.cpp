#include "Internal.hpp"

#include <set>

namespace mq::kernel::pitch::request::detail {
namespace {

Error failure(Error::Code code, std::string message) {
    return {
        code,
        std::move(message),
        std::nullopt,
        std::nullopt,
        std::nullopt,
    };
}

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

bool valid(motion::Direction direction) {
    switch (direction) {
    case motion::Direction::Start:
    case motion::Direction::Same:
    case motion::Direction::Rise:
    case motion::Direction::Fall:
        return true;
    }
    return false;
}

} // namespace

std::expected<const performance::Event*, Error> check(
    const state::Snapshot& state,
    const performance::Plan& prefix,
    std::size_t limit) {
    if (state.melody.history.size() > limit ||
        prefix.events.size() > limit) {
        return std::unexpected(failure(
            Error::Code::Count,
            "pitch request history budget exceeded"));
    }
    if (!state.melody.current || state.melody.history.empty()) {
        return std::unexpected(failure(
            Error::Code::State,
            "pitch request requires a current structural event"));
    }
    if (*state.melody.current != state.melody.history.back()) {
        return std::unexpected(failure(
            Error::Code::State,
            "current structural event disagrees with event history"));
    }
    std::set<Identity> identities;
    for (std::size_t index = 0;
         index < state.melody.history.size();
         ++index) {
        const auto& event = state.melody.history[index];
        const bool starts =
            event.direction == motion::Direction::Start;
        if (!complete(event.identity) ||
            !complete(event.role) ||
            !valid(event.direction) ||
            !identities.insert(event.identity).second ||
            starts != (index == 0)) {
            return std::unexpected(failure(
                Error::Code::State,
                "structural event history violates identity or direction laws"));
        }
    }
    if (!prefix.well_formed()) {
        return std::unexpected(failure(
            Error::Code::Plan,
            "performance plan timing is not contiguous or valid"));
    }
    if (prefix.events.size() + 1 != state.melody.history.size()) {
        return std::unexpected(failure(
            Error::Code::History,
            "performance plan is not the exact event-history prefix"));
    }
    for (std::size_t index = 0; index < prefix.events.size(); ++index) {
        if (prefix.events[index].target.event !=
            state.melody.history[index]) {
            return std::unexpected(failure(
                Error::Code::History,
                "performance plan event does not match event history"));
        }
    }

    return &*state.melody.current;
}

} // namespace mq::kernel::pitch::request::detail
