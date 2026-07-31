#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/jins/Descriptor.hpp"

#include <expected>
#include <string>
#include <variant>
#include <vector>

namespace mq::kernel::jins {

struct Overlay {
    struct DefineRole { Identity role; };
    struct ReplaceTonic { Identity role; };
    struct RequireGhammaz { Identity role; };
    struct PermitRegion { Identity region; };
    struct MarkCharacteristic { Identity region; };
    struct RequireEmphasis { Identity role; };
    struct ProhibitRole { Identity role; };
    struct ProhibitGhammaz { Identity role; };
    struct ProhibitRegion { Identity region; };
    struct UnmarkCharacteristic { Identity region; };
    struct ProhibitEmphasis { Identity role; };
    struct AddBaggage { Baggage baggage; };
    struct RemoveBaggage { Identity baggage; };
    struct AddGesture { gesture::Definition gesture; };
    struct RemoveGesture { Identity gesture; };
    struct DefineEntry { Identity gesture; };
    struct ProhibitEntry { Identity gesture; };
    struct DefineExit { Identity gesture; };
    struct ProhibitExit { Identity gesture; };
    struct DefineCadence { Identity gesture; };
    struct ProhibitCadence { Identity gesture; };
    struct DefineMotif { Identity gesture; };
    struct ProhibitMotif { Identity gesture; };

    using Action = std::variant<
        DefineRole,
        ReplaceTonic,
        RequireGhammaz,
        PermitRegion,
        MarkCharacteristic,
        RequireEmphasis,
        ProhibitRole,
        ProhibitGhammaz,
        ProhibitRegion,
        UnmarkCharacteristic,
        ProhibitEmphasis,
        AddBaggage,
        RemoveBaggage,
        AddGesture,
        RemoveGesture,
        DefineEntry,
        ProhibitEntry,
        DefineExit,
        ProhibitExit,
        DefineCadence,
        ProhibitCadence,
        DefineMotif,
        ProhibitMotif>;

    Action action;
    std::vector<std::string> provenance;
};

[[nodiscard]] std::expected<Descriptor, std::string> reconstruct(
    Descriptor base,
    const std::vector<Overlay>& overlays);

} // namespace mq::kernel::jins
