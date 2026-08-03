#pragma once

#include "mq/kernel/maqam/Family.hpp"

#include <optional>
#include <map>
#include <vector>

namespace mq::kernel::maqam::family::detail {

struct BranchKey {
    std::string name;
    Identity jins;
    Identity source_center;
    Identity center;
    Identity tonic;
    Identity ghammaz;
    Identity descent;
    Identity motion;
    motion::Direction direction{motion::Direction::Fall};
    Identity path;
    Identity travel;
    Identity restore;
    Identity route;
    pitch::Expression target;
    std::string source;
};

struct RouteKey {
    Identity route;
    std::vector<std::size_t> branches;
    struct Step {
        std::string name;
        std::optional<std::size_t> branch;
        std::vector<ActionSpec> actions;
        std::optional<std::vector<std::size_t>> next;
        std::string transition_provenance;
        std::size_t minimum{1};
        std::size_t maximum{1};
    };
    std::vector<Step> steps;
    std::vector<Identity> terminals;
    std::size_t variants{1};
    bool valid{true};
};

struct FormulaKey {
    std::string name;
    Identity identity;
    Identity cell;
    Identity motif;
    std::string provenance;
    std::vector<FormulaNoteSpec> notes;
};

struct FormulaVariationKey {
    Identity base;
    Identity variation;
    std::string transformation;
    std::string provenance;
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
    std::vector<Identity> rootRoles;
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
    std::map<std::string, Identity> authorities;
    std::vector<JinsSpec> jins;
    std::vector<GestureSpec> gestures;
    std::vector<BaggageSpec> baggage;
    std::vector<ObligationSpec> obligations;
    std::vector<FormulaKey> formulas;
    std::vector<FormulaVariationKey> formula_variations;
    pitch::Expression ghammaz;
    pitch::Expression extension;
    std::vector<BranchKey> branches;
    std::vector<RouteKey> routes;
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
} // namespace mq::kernel::maqam::family::detail
