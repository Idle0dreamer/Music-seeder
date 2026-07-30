#include "Internal.hpp"

#include <type_traits>

namespace mq::kernel::pitch::field::project::detail {
namespace {

std::expected<std::optional<Fact>, Error> absent(
    const Identity& key,
    Presence presence) {
    if (presence == Presence::Required) {
        return std::unexpected(Error{
            Error::Code::Missing,
            "required field projection state is missing",
            key,
        });
    }
    return std::optional<Fact>{};
}

} // namespace

std::expected<std::optional<Fact>, Error> read(
    const Source& source,
    const state::Snapshot& state) {
    return std::visit(
        [&](const auto& reader)
            -> std::expected<std::optional<Fact>, Error> {
            using Type = std::decay_t<decltype(reader)>;
            if constexpr (std::same_as<Type, center::Read>) {
                if (state.center.stack.empty()) {
                    return absent(reader.key, reader.presence);
                }
                const auto& value =
                    reader.place == center::Place::Root
                        ? state.center.stack.front()
                        : state.center.stack.back();
                return std::optional<Fact>{{reader.key, value}};
            } else if constexpr (std::same_as<Type, jins::Read>) {
                if (!state.jins.active) {
                    return absent(reader.key, reader.presence);
                }
                return std::optional<Fact>{{
                    reader.key,
                    *state.jins.active,
                }};
            } else if constexpr (std::same_as<Type, role::Read>) {
                if (!state.melody.current) {
                    return absent(reader.key, reader.presence);
                }
                return std::optional<Fact>{{
                    reader.key,
                    state.melody.current->role,
                }};
            } else if constexpr (std::same_as<Type, motion::Read>) {
                if (!state.melody.current) {
                    return absent(reader.key, reader.presence);
                }
                const Identity* value = &reader.start;
                switch (state.melody.current->direction) {
                case mq::kernel::motion::Direction::Start:
                    break;
                case mq::kernel::motion::Direction::Same:
                    value = &reader.same;
                    break;
                case mq::kernel::motion::Direction::Rise:
                    value = &reader.rise;
                    break;
                case mq::kernel::motion::Direction::Fall:
                    value = &reader.fall;
                    break;
                default:
                    return std::unexpected(Error{
                        Error::Code::Input,
                        "current event has an invalid direction",
                        reader.key,
                    });
                }
                return std::optional<Fact>{{reader.key, *value}};
            } else if constexpr (
                std::same_as<Type, tonicization::Read>) {
                const Identity* value = &reader.color;
                if (state.tonicization.level ==
                    mq::kernel::tonicization::Level::Internal) {
                    value = &reader.internal;
                } else if (state.tonicization.level ==
                           mq::kernel::tonicization::Level::Maqam) {
                    value = &reader.maqam;
                }
                return std::optional<Fact>{{reader.key, *value}};
            } else if constexpr (std::same_as<Type, path::Read>) {
                const bool completed =
                    state.path.completed.contains(reader.path);
                return std::optional<Fact>{{
                    reader.key,
                    completed ? reader.complete : reader.incomplete,
                }};
            } else {
                return read(reader, state);
            }
        },
        source);
}

} // namespace mq::kernel::pitch::field::project::detail
