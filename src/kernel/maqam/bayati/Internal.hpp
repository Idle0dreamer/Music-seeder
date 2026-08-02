#pragma once

#include "mq/kernel/maqam/Bayati.hpp"
#include "mq/kernel/generate/Stage.hpp"

#include <array>
#include <string>

namespace mq::kernel::maqam::bayati::detail {

struct Key {
    Identity choice;
    Identity centerRoot;
    Identity centerUpper;
    Identity jinsRoot;
    Identity jinsNahawand;
    Identity jinsRast;
    Identity jinsHijaz;
    Identity roleTonic;
    Identity roleSikah;
    Identity roleJaharkah;
    Identity roleGhammaz;
    Identity roleHusayni;
    Identity roleExtension;
    Identity regionRoot;
    Identity regionUpper;
    Identity gestureEstablish;
    Identity gestureAscent;
    Identity gestureDescent;
    Identity gestureResolution;
    Identity phraseEstablish;
    Identity phraseReturn;
    Identity cellEstablish;
    Identity cellDevelop;
    Identity cellClimax;
    Identity cellReturn;
    Identity cadenceLocal;
    Identity cadenceReturn;
    Identity baggageExtension;
    Identity pathNahawand;
    Identity pathRast;
    Identity pathHijaz;
    Identity obligationEstablish;
    Identity obligationSettle;
    Identity obligationExpand;
    Identity obligationClimax;
    Identity obligationTravelNahawand;
    Identity obligationTravelRast;
    Identity obligationTravelHijaz;
    Identity obligationRestoreNahawand;
    Identity obligationRestoreRast;
    Identity obligationRestoreHijaz;
    Identity routeStay;
    Identity routeNahawand;
    Identity routeRast;
    Identity routeHijaz;
    Identity keyRole;
    Identity keyJins;
    Identity keyMotion;
    Identity keyRegion;
    Identity keyBaggage;
    Identity keyGesture;
    Identity keyFunction;
    Identity keyCadence;
    Identity keyBoundary;
    Identity motionStart;
    Identity motionSame;
    Identity motionRise;
    Identity motionFall;
    Identity boundaryOpen;
    Identity boundaryClosed;
};

[[nodiscard]] Identity id(std::string name);
[[nodiscard]] Key keys();

[[nodiscard]] std::expected<std::shared_ptr<profile::Set>, std::string>
profile(const Key& key);

[[nodiscard]] std::expected<jins::Catalog, std::string>
ajnas(const Key& key);

[[nodiscard]] std::expected<path::Graph, std::string>
paths(const Key& key);

[[nodiscard]] std::expected<sayr::Plan, std::string>
sayr(const Key& key);

[[nodiscard]] performance::Timing timing();

[[nodiscard]] std::expected<Generation, std::string>
generation(const Key& key);

} // namespace mq::kernel::maqam::bayati::detail
