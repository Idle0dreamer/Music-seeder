#pragma once

#include "mq/kernel/maqam/Rast.hpp"

namespace mq::kernel::maqam::rast::detail {

Identity id(std::string name);

struct Key {
    Identity choice;
    Identity centerRoot;
    Identity centerUpper;
    Identity jinsRoot;
    Identity jinsUpperRast;
    Identity jinsUpperNahawand;
    Identity roleTonic;
    Identity roleGhammaz;
    Identity roleUpper;
    Identity roleExtension;
    Identity regionRoot;
    Identity regionUpper;
    Identity gestureEstablish;
    Identity gestureAscent;
    Identity gestureDescent;
    Identity gestureResolution;
    Identity phraseQuestion;
    Identity phraseResponse;
    Identity cellEstablish;
    Identity cellDevelop;
    Identity cellClimax;
    Identity cellReturn;
    Identity formulaEstablish;
    Identity formulaDevelop;
    Identity formulaDevelopVariation;
    Identity formulaClimax;
    Identity formulaReturn;
    Identity cadenceLocal;
    Identity cadenceReturn;
    Identity baggageExtension;
    Identity pathUpperRast;
    Identity pathNahawand;
    Identity obligationEstablish;
    Identity obligationSettle;
    Identity obligationExpand;
    Identity obligationClimax;
    Identity obligationTravelUpperRast;
    Identity obligationTravelNahawand;
    Identity obligationRestoreUpperRast;
    Identity obligationRestoreNahawand;
    Identity routeStay;
    Identity routeUpperRast;
    Identity routeNahawand;
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

[[nodiscard]] Key keys();
[[nodiscard]] std::expected<jins::Catalog, std::string> ajnas(const Key&);
[[nodiscard]] std::expected<path::Graph, std::string> paths(const Key&);
[[nodiscard]] std::expected<sayr::Plan, std::string> sayr(const Key&);
[[nodiscard]] std::expected<std::shared_ptr<profile::Set>, std::string>
profile(const Key&);
[[nodiscard]] std::expected<Generation, std::string>
generation(const Key&);


} // namespace mq::kernel::maqam::rast::detail
