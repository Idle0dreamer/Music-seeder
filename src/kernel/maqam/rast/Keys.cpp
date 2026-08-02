#include "Internal.hpp"

namespace mq::kernel::maqam::rast::detail {

Identity id(std::string name) {
    return {"maqam.rast", std::move(name), "1"};
}

Key keys() {
    return {
        id("choice"),
        id("center.root"),
        id("center.upper"),
        id("jins.rast.root"),
        id("jins.rast.upper"),
        id("jins.nahawand.upper"),
        id("role.tonic"),
        id("role.ghammaz"),
        id("role.upper"),
        id("role.extension"),
        id("region.root"),
        id("region.upper"),
        id("gesture.establish"),
        id("gesture.ascent"),
        id("gesture.descent"),
        id("gesture.resolution"),
        id("phrase.question"),
        id("phrase.response"),
        id("cell.establish"),
        id("cell.develop"),
        id("cell.climax"),
        id("cell.return"),
        id("formula.establish"),
        id("formula.develop"),
        id("formula.develop-variation"),
        id("formula.climax"),
        id("formula.return"),
        id("cadence.local"),
        id("cadence.return"),
        id("baggage.extension"),
        id("path.to.upper-rast"),
        id("path.to.nahawand"),
        id("obligation.establish"),
        id("obligation.settle"),
        id("obligation.expand"),
        id("obligation.climax"),
        id("obligation.travel.upper-rast"),
        id("obligation.travel.nahawand"),
        id("obligation.restore.upper-rast"),
        id("obligation.restore.nahawand"),
        id("route.stay"),
        id("route.upper-rast"),
        id("route.nahawand"),
        id("key.role"),
        id("key.jins"),
        id("key.motion"),
        id("key.region"),
        id("key.baggage"),
        id("key.gesture"),
        id("key.function"),
        id("key.cadence"),
        id("key.boundary"),
        id("motion.start"),
        id("motion.same"),
        id("motion.rise"),
        id("motion.fall"),
        id("boundary.open"),
        id("boundary.closed"),
    };
}

} // namespace mq::kernel::maqam::rast::detail
