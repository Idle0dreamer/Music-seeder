#include "../../../Test.hpp"

#include "mq/kernel/pitch/field/project/Run.hpp"

#include <algorithm>

namespace {

mq::kernel::Identity id(
    std::string domain,
    std::string name) {
    return {std::move(domain), std::move(name), "1"};
}

std::optional<mq::kernel::Identity> value(
    const mq::kernel::pitch::field::Context& context,
    const mq::kernel::Identity& key) {
    const auto found = std::ranges::find(
        context.facts,
        key,
        &mq::kernel::pitch::field::Fact::key);
    if (found == context.facts.end()) {
        return std::nullopt;
    }
    return found->value;
}

} // namespace

void test::project::run() {
    using namespace mq::kernel;
    namespace pp = pitch::field::project;

    const auto root = id("test.project.center", "root");
    const auto local = id("test.project.center", "local");
    const auto jins = id("test.project.jins", "active");
    const auto route = id("test.project.path", "route");
    const auto yes = id("test.project.value", "yes");
    const auto no = id("test.project.value", "no");
    const auto color = id("test.project.value", "color");
    const auto internal = id("test.project.value", "internal");
    const auto maqam = id("test.project.value", "maqam");
    const auto centerRoot = id("test.project.key", "center.root");
    const auto centerLocal = id("test.project.key", "center.local");
    const auto jinsKey = id("test.project.key", "jins");
    const auto levelKey = id("test.project.key", "level");
    const auto pathKey = id("test.project.key", "path");
    const auto roleKey = id("test.project.key", "role");
    const auto motionKey = id("test.project.key", "motion");
    const auto role = id("test.project.role", "current");
    const auto event = id("test.project.event", "current");
    const auto region = id("test.project.register", "upper");
    const auto baggage = id("test.project.baggage", "extension");
    const auto gesture = id("test.project.gesture", "ascent");
    const auto regionKey = id("test.project.key", "register");
    const auto baggageKey = id("test.project.key", "baggage");
    const auto gestureKey = id("test.project.key", "gesture");
    const auto start = id("test.project.motion", "start");
    const auto same = id("test.project.motion", "same");
    const auto rise = id("test.project.motion", "rise");
    const auto fall = id("test.project.motion", "fall");

    state::Snapshot state;
    state.center.stack = {sort::CenterId{root}, sort::CenterId{local}};
    state.jins.active = sort::JinsId{jins};
    state.tonicization.level = tonicization::Level::Internal;
    state.path.completed.insert(sort::PathId{route});
    state.melody.current = performance::Event{
        event,
        role,
        motion::Direction::Rise,
        region,
        baggage,
        performance::Gesture{gesture, gesture},
    };
    state.melody.history.push_back(*state.melody.current);
    pp::Plan plan{{
        pp::path::Read{pathKey, route, yes, no},
        pp::tonicization::Read{levelKey, color, internal, maqam},
        pp::center::Read{centerLocal, pp::center::Place::Local},
        pp::jins::Read{jinsKey},
        pp::center::Read{centerRoot, pp::center::Place::Root},
        pp::role::Read{roleKey},
        pp::registral::Read{regionKey},
        pp::baggage::Read{baggageKey},
        pp::gesture::Read{gestureKey},
        pp::motion::Read{
            motionKey,
            start,
            same,
            rise,
            fall,
        },
    }};
    const auto projected = pp::run(plan, state);
    require(
        projected &&
            value(*projected, centerRoot) == root &&
            value(*projected, centerLocal) == local &&
            value(*projected, jinsKey) == jins &&
            value(*projected, levelKey) == internal &&
            value(*projected, pathKey) == yes &&
            value(*projected, roleKey) == role &&
            value(*projected, regionKey) == region &&
            value(*projected, baggageKey) == baggage &&
            value(*projected, gestureKey) == gesture &&
            value(*projected, motionKey) == rise,
        "typed state projection produced wrong field facts");

    std::ranges::reverse(plan.sources);
    const auto reordered = pp::run(plan, state);
    require(
        reordered && reordered->facts == projected->facts,
        "projection source storage order changed context");

    const pp::Plan required{{pp::jins::Read{jinsKey}}};
    const auto missing = pp::run(required, {});
    require(
        !missing && missing.error().code == pp::Error::Code::Missing,
        "missing required state was omitted");
    const pp::Plan optional{{
        pp::jins::Read{jinsKey, pp::Presence::Optional},
    }};
    const auto omitted = pp::run(optional, {});
    require(
        omitted && omitted->facts.empty(),
        "missing optional state did not omit its fact");

    const auto limited = pp::run(plan, state, pp::Limits{2});
    require(
        !limited && limited.error().code == pp::Error::Code::Count,
        "projection source budget exhaustion did not fail");
}
