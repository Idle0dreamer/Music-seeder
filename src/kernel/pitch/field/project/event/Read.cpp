#include "../Internal.hpp"

namespace mq::kernel::pitch::field::project::detail {
namespace {

std::expected<std::optional<Fact>, Error> absent(
    const Identity& key,
    Presence presence) {
    if (presence == Presence::Required) {
        return std::unexpected(Error{
            Error::Code::Missing,
            "required structural event context is missing",
            key,
        });
    }
    return std::optional<Fact>{};
}

} // namespace

std::expected<std::optional<Fact>, Error> read(
    const registral::Read& source,
    const state::Snapshot& state) {
    if (!state.melody.current) {
        return absent(source.key, source.presence);
    }
    return std::optional<Fact>{{
        source.key,
        state.melody.current->region,
    }};
}

std::expected<std::optional<Fact>, Error> read(
    const baggage::Read& source,
    const state::Snapshot& state) {
    if (!state.melody.current || !state.melody.current->baggage) {
        return absent(source.key, source.presence);
    }
    return std::optional<Fact>{{
        source.key,
        *state.melody.current->baggage,
    }};
}

std::expected<std::optional<Fact>, Error> read(
    const gesture::Read& source,
    const state::Snapshot& state) {
    if (!state.melody.current || !state.melody.current->gesture) {
        return absent(source.key, source.presence);
    }
    return std::optional<Fact>{{
        source.key,
        state.melody.current->gesture->family,
    }};
}

} // namespace mq::kernel::pitch::field::project::detail
