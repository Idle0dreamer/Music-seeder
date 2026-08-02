#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::maqam::family::detail {

Identity id(const Key& key, std::string name) {
    return {"maqam." + key.package, std::move(name), "1"};
}

Key key(const Spec& spec) {
    Key result{};
    result.package = spec.package;
    result.family = spec.family;
    result.source = spec.provenance;
    result.ghammaz = spec.ghammaz;
    result.extension = spec.extension;
    result.choice = id(result, "choice");
    result.centerRoot = id(result, "center.root");
    result.centerUpper = id(result, "center.upper");
    result.jinsRoot = id(result, "jins." + spec.package + ".root");
    result.roleTonic = id(result, "role.tonic");
    result.roleGhammaz = id(result, "role.ghammaz");
    result.roleUpper = id(
        result,
        "role." +
            (spec.upper_role.empty() ? std::string("upper") :
                                        spec.upper_role));
    result.roleExtension = id(result, "role.extension");
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
            result.rootRoles.push_back(id(result, "role." + name));
        }
    }
    result.regionRoot = id(result, "region.root");
    result.regionUpper = id(result, "region.upper");
    result.gestureEstablish = id(result, "gesture.establish");
    result.gestureAscent = id(result, "gesture.ascent");
    result.gestureDescent = id(result, "gesture.descent");
    result.gestureResolution = id(result, "gesture.resolution");
    result.phraseQuestion = id(result, "phrase.question");
    result.phraseResponse = id(result, "phrase.response");
    result.cellEstablish = id(result, "cell.establish");
    result.cellDevelop = id(result, "cell.develop");
    result.cellClimax = id(result, "cell.climax");
    result.cellReturn = id(result, "cell.return");
    result.formulaEstablish = id(result, "formula.establish");
    result.formulaDevelop = id(result, "formula.develop");
    result.formulaDevelopVariation = id(result, "formula.develop-variation");
    result.formulaClimax = id(result, "formula.climax");
    result.formulaReturn = id(result, "formula.return");
    result.cadenceLocal = id(result, "cadence.local");
    result.cadenceReturn = id(result, "cadence.return");
    result.baggageExtension = id(result, "baggage.extension");
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
    result.ordered = spec.ordered;
    result.branches.reserve(spec.branches.size());
    for (const auto& source : spec.branches) {
        BranchKey branch;
        branch.jins = id(result, "jins." + source.name + ".upper");
        branch.source_center = source.source_center_name.empty()
                                   ? result.centerRoot
                                   : id(result, "center." + source.source_center_name);
        branch.center = source.center_name.empty()
                            ? result.centerUpper
                            : id(result, "center." + source.center_name);
        branch.tonic = source.tonic_role.empty()
                           ? result.roleGhammaz
                           : id(result, "role." + source.tonic_role);
        branch.ghammaz = source.ghammaz_role.empty()
                             ? result.roleUpper
                             : id(result, "role." + source.ghammaz_role);
        branch.descent = source.descent_role.empty()
                             ? branch.ghammaz
                             : id(result, "role." + source.descent_role);
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
    return result;
}

} // namespace mq::kernel::maqam::family::detail
