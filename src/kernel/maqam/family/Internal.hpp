#pragma once

#include "mq/kernel/maqam/Family.hpp"

#include <vector>

namespace mq::kernel::maqam::family::detail {

struct BranchKey {
    Identity jins;
    Identity path;
    Identity travel;
    Identity restore;
    Identity route;
    pitch::Expression target;
    std::string source;
};

struct Key {
    std::string package;
    std::string family;
    std::string source;
    Identity choice;
    Identity centerRoot;
    Identity centerUpper;
    Identity jinsRoot;
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
    pitch::Expression ghammaz;
    pitch::Expression extension;
    std::vector<BranchKey> branches;
    performance::Timing timing;
};

[[nodiscard]] Identity id(const Key&, std::string name);
[[nodiscard]] Key key(const Spec&);
[[nodiscard]] std::expected<jins::Catalog, std::string> ajnas(const Key&);
[[nodiscard]] std::expected<path::Graph, std::string> paths(const Key&);
[[nodiscard]] std::expected<sayr::Plan, std::string> sayr(const Key&);
[[nodiscard]] std::expected<std::shared_ptr<profile::Set>, std::string>
profile(const Key&);
[[nodiscard]] std::expected<Generation, std::string>
generation(const Key&);
[[nodiscard]] performance::Timing timing(const Key&);

} // namespace mq::kernel::maqam::family::detail
