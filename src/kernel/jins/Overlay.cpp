#include "mq/kernel/jins/Overlay.hpp"
#include "Internal.hpp"

namespace mq::kernel::jins {

std::expected<Descriptor, std::string> reconstruct(
    Descriptor base,
    const std::vector<Overlay>& overlays) {
    for (const auto& overlay : overlays) {
        std::visit(
            [&base](const auto& action) {
                using T = std::decay_t<decltype(action)>;
                if constexpr (std::is_same_v<T, Overlay::DefineRole>) {
                    base.roles.insert(action.role);
                } else if constexpr (std::is_same_v<T, Overlay::ReplaceTonic>) {
                    base.tonic = action.role;
                } else if constexpr (std::is_same_v<T, Overlay::RequireGhammaz>) {
                    base.ghammaz.insert(action.role);
                } else if constexpr (std::is_same_v<T, Overlay::PermitRegion>) {
                    base.regions.insert(action.region);
                } else if constexpr (std::is_same_v<T, Overlay::MarkCharacteristic>) {
                    base.characteristic.insert(action.region);
                } else if constexpr (std::is_same_v<T, Overlay::RequireEmphasis>) {
                    base.emphasis.insert(action.role);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitRole>) {
                    base.roles.erase(action.role);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitGhammaz>) {
                    base.ghammaz.erase(action.role);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitRegion>) {
                    base.regions.erase(action.region);
                } else if constexpr (std::is_same_v<T, Overlay::UnmarkCharacteristic>) {
                    base.characteristic.erase(action.region);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitEmphasis>) {
                    base.emphasis.erase(action.role);
                } else if constexpr (std::is_same_v<T, Overlay::AddBaggage>) {
                    base.baggage[action.baggage.identity] = action.baggage;
                } else if constexpr (std::is_same_v<T, Overlay::RemoveBaggage>) {
                    base.baggage.erase(action.baggage);
                } else if constexpr (std::is_same_v<T, Overlay::AddGesture>) {
                    base.gestures[action.gesture.identity] = action.gesture;
                } else if constexpr (std::is_same_v<T, Overlay::RemoveGesture>) {
                    base.gestures.erase(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::DefineEntry>) {
                    base.entry.insert(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitEntry>) {
                    base.entry.erase(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::DefineExit>) {
                    base.exit.insert(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitExit>) {
                    base.exit.erase(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::DefineCadence>) {
                    base.cadences.insert(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitCadence>) {
                    base.cadences.erase(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::DefineMotif>) {
                    base.motifs.insert(action.gesture);
                } else if constexpr (std::is_same_v<T, Overlay::ProhibitMotif>) {
                    base.motifs.erase(action.gesture);
                }
        },
            overlay.action);
        base.provenance.insert(
            base.provenance.end(),
            overlay.provenance.begin(),
            overlay.provenance.end());
    }
    const auto valid = detail::validate(base);
    if (!valid) {
        return std::unexpected(valid.error());
    }
    return base;
}

} // namespace mq::kernel::jins
