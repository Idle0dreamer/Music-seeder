#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::pitch::field::project::detail {

std::expected<std::optional<Fact>, Error> read(
    const project::phrase::function::Read& source,
    const state::Snapshot& state) {
    if (const auto* frame = phrase::active(state)) {
        return std::optional<Fact>{{
            source.key,
            frame->function.identity,
        }};
    }
    const auto span = phrase::completed(state);
    if (!span) {
        return std::unexpected(span.error());
    }
    if (*span == nullptr) {
        return phrase::absent(source.key, source.presence);
    }
    return std::optional<Fact>{{
        source.key,
        (*span)->function.identity,
    }};
}

std::expected<std::optional<Fact>, Error> read(
    const project::phrase::cadence::Read& source,
    const state::Snapshot& state) {
    const auto event = phrase::current(state);
    if (!event) {
        return std::unexpected(event.error());
    }
    if (*event == nullptr) {
        return phrase::absent(source.key, source.presence);
    }

    const std::vector<mq::kernel::phrase::Cadence>* cadences = nullptr;
    if (const auto* frame = phrase::active(state)) {
        cadences = &frame->cadences;
    } else {
        const auto span = phrase::completed(state);
        if (!span) {
            return std::unexpected(span.error());
        }
        if (*span != nullptr) {
            cadences = &(*span)->cadences;
        }
    }
    if (cadences == nullptr) {
        return phrase::absent(source.key, source.presence);
    }
    const auto found = std::ranges::find(
        *cadences,
        (*event)->identity,
        &mq::kernel::phrase::Cadence::event);
    if (found == cadences->end()) {
        return phrase::absent(source.key, source.presence);
    }
    return std::optional<Fact>{{source.key, found->family}};
}

std::expected<std::optional<Fact>, Error> read(
    const project::phrase::boundary::Read& source,
    const state::Snapshot& state) {
    const auto span = phrase::completed(state);
    if (!span) {
        return std::unexpected(span.error());
    }
    if (*span == nullptr) {
        return phrase::absent(source.key, source.presence);
    }
    const auto value =
        (*span)->boundary == mq::kernel::phrase::Boundary::Open
            ? source.open
            : source.closed;
    return std::optional<Fact>{{source.key, value}};
}

} // namespace mq::kernel::pitch::field::project::detail
