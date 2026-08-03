#include "Internal.hpp"

#include <algorithm>
#include <iterator>
#include <string_view>

namespace mq::kernel::maqam::family::detail {

Identity id(const Key& key, std::string name) {
    return {"maqam." + key.package, std::move(name), "1"};
}

Identity declared(
    const Key& key,
    std::string_view kind,
    std::string_view name,
    Identity fallback) {
    const auto found = key.authorities.find(
        std::string(kind) + "." + std::string(name));
    return found == key.authorities.end() ? std::move(fallback) : found->second;
}

Key key(const Spec& spec) {
    Key result{};
    result.package = spec.package;
    result.family = spec.family;
    result.source = spec.provenance;
    result.ghammaz = spec.ghammaz;
    result.extension = spec.extension;
    for (const auto& authority : spec.authorities) {
        for (const auto& name : authority.names) {
            const auto token = authority.kind + "." + name;
            const auto identityName = authority.kind == "jins" && name == "root"
                                          ? "jins." + spec.package + ".root"
                                          : token;
            result.authorities.emplace(
                token,
                id(result, identityName));
        }
    }
    result.choice = id(result, "choice");
    result.centerRoot = declared(
        result, "center", "root", id(result, "center.root"));
    result.centerUpper = declared(
        result, "center", "upper", id(result, "center.upper"));
    result.jinsRoot = declared(
        result, "jins", "root", id(result, "jins." + spec.package + ".root"));
    result.roleTonic = declared(
        result, "role", "tonic", id(result, "role.tonic"));
    result.roleGhammaz = declared(
        result, "role", "ghammaz", id(result, "role.ghammaz"));
    result.roleUpper = id(
        result, "role." +
                    (spec.upper_role.empty() ? std::string("upper") :
                                                spec.upper_role));
    result.roleUpper = declared(
        result,
        "role",
        spec.upper_role.empty() ? "upper" : spec.upper_role,
        result.roleUpper);
    result.roleExtension = declared(
        result, "role", "extension", id(result, "role.extension"));
    if (spec.root_roles.empty()) {
        result.rootRoles = {
            result.roleTonic,
            result.roleGhammaz,
            result.roleUpper,
            result.roleExtension,
        };
    } else {
        result.rootRoles.reserve(spec.root_roles.size());
        for (const auto& name : spec.root_roles) {
            if (name.empty()) {
                continue;
            }
            result.rootRoles.push_back(declared(
                result, "role", name, id(result, "role." + name)));
        }
    }
    result.regionRoot = declared(
        result, "region", "root", id(result, "region.root"));
    result.regionUpper = declared(
        result, "region", "upper", id(result, "region.upper"));
    result.gestureEstablish = declared(
        result, "gesture", "establish", id(result, "gesture.establish"));
    result.gestureAscent = declared(
        result, "gesture", "ascent", id(result, "gesture.ascent"));
    result.gestureDescent = declared(
        result, "gesture", "descent", id(result, "gesture.descent"));
    result.gestureResolution = declared(
        result, "gesture", "resolution", id(result, "gesture.resolution"));
    result.phraseQuestion = declared(
        result, "phrase", "question", id(result, "phrase.question"));
    result.phraseResponse = declared(
        result, "phrase", "response", id(result, "phrase.response"));
    result.cellEstablish = declared(
        result, "cell", "establish", id(result, "cell.establish"));
    result.cellDevelop = declared(
        result, "cell", "develop", id(result, "cell.develop"));
    result.cellClimax = declared(
        result, "cell", "climax", id(result, "cell.climax"));
    result.cellReturn = declared(
        result, "cell", "return", id(result, "cell.return"));
    result.formulaEstablish = declared(
        result, "formula", "establish", id(result, "formula.establish"));
    result.formulaDevelop = declared(
        result, "formula", "develop", id(result, "formula.develop"));
    result.formulaDevelopVariation = declared(
        result,
        "formula",
        "develop-variation",
        id(result, "formula.develop-variation"));
    result.formulaClimax = declared(
        result, "formula", "climax", id(result, "formula.climax"));
    result.formulaReturn = declared(
        result, "formula", "return", id(result, "formula.return"));
    result.cadenceLocal = declared(
        result, "cadence", "local", id(result, "cadence.local"));
    result.cadenceReturn = declared(
        result, "cadence", "return", id(result, "cadence.return"));
    result.baggageExtension = declared(
        result, "baggage", "extension", id(result, "baggage.extension"));
    result.keyRole = id(result, "key.role");
    result.keyJins = id(result, "key.jins");
    result.keyMotion = id(result, "key.motion");
    result.keyRegion = id(result, "key.region");
    result.keyBaggage = id(result, "key.baggage");
    result.keyGesture = id(result, "key.gesture");
    result.keyFunction = id(result, "key.function");
    result.keyCadence = id(result, "key.cadence");
    result.keyBoundary = id(result, "key.boundary");
    result.motionStart = id(result, "motion.start");
    result.motionSame = id(result, "motion.same");
    result.motionRise = id(result, "motion.rise");
    result.motionFall = id(result, "motion.fall");
    result.boundaryOpen = id(result, "boundary.open");
    result.boundaryClosed = id(result, "boundary.closed");
    result.branches.reserve(spec.branches.size());
    for (const auto& source : spec.branches) {
        BranchKey branch;
        branch.name = source.name;
        branch.jins = id(result, "jins." + source.name + ".upper");
        branch.source_center = source.source_center_name.empty()
                                   ? result.centerRoot
                                   : declared(result, "center", source.source_center_name,
                                              id(result, "center." + source.source_center_name));
        branch.center = source.center_name.empty()
                            ? result.centerUpper
                            : declared(result, "center", source.center_name,
                                       id(result, "center." + source.center_name));
        branch.tonic = source.tonic_role.empty()
                           ? result.roleGhammaz
                           : declared(result, "role", source.tonic_role,
                                      id(result, "role." + source.tonic_role));
        branch.ghammaz = source.ghammaz_role.empty()
                             ? result.roleUpper
                             : declared(result, "role", source.ghammaz_role,
                                        id(result, "role." + source.ghammaz_role));
        branch.descent = source.descent_role.empty()
                             ? branch.ghammaz
                             : declared(result, "role", source.descent_role,
                                        id(result, "role." + source.descent_role));
        branch.direction = source.direction;
        branch.motion = source.direction == motion::Direction::Rise
                            ? result.motionRise
                            : result.motionFall;
        branch.path = id(result, "path.to." + source.name);
        branch.travel = id(result, "obligation.travel." + source.name);
        branch.restore = id(result, "obligation.restore." + source.name);
        branch.route = id(result, "route." + source.name);
        branch.target = source.target;
        branch.source = source.provenance;
        result.branches.push_back(std::move(branch));
    }
    result.routes.reserve(spec.routes.size());
    for (const auto& source : spec.routes) {
        RouteKey route;
        route.route = id(result, "route." + source.name);
        route.variants = source.variants;
        for (const auto& branch : source.branches) {
            const auto found = std::ranges::find_if(
                result.branches,
                [&](const auto& value) { return value.name == branch; });
            if (found == result.branches.end()) {
                route.valid = false;
                continue;
            }
            route.branches.push_back(
                static_cast<std::size_t>(std::distance(result.branches.begin(), found)));
        }
        for (const auto& step : source.steps) {
            RouteKey::Step translated{
                step.name,
                std::nullopt,
                step.actions};
            translated.minimum = step.minimum;
            translated.maximum = step.maximum;
            if (!step.branch.empty()) {
                const auto found = std::ranges::find_if(
                    result.branches,
                    [&](const auto& value) { return value.name == step.branch; });
                if (found == result.branches.end()) {
                    route.valid = false;
                } else {
                    translated.branch = static_cast<std::size_t>(
                        std::distance(result.branches.begin(), found));
                }
            }
            route.steps.push_back(std::move(translated));
        }
        result.routes.push_back(std::move(route));
    }
    return result;
}

} // namespace mq::kernel::maqam::family::detail
