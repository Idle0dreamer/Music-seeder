#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::jins::detail {
namespace {

bool subset(
    const std::set<Identity>& part,
    const std::set<Identity>& whole) {
    return std::ranges::all_of(
        part,
        [&](const auto& item) {
            return whole.contains(item);
        });
}

std::expected<void, std::string> fail(std::string message) {
    return std::unexpected(std::move(message));
}

} // namespace

bool complete(const Identity& identity) {
    return !identity.domain.empty() &&
           !identity.name.empty() &&
           !identity.revision.empty();
}

bool valid(motion::Direction direction) {
    switch (direction) {
    case motion::Direction::Start:
    case motion::Direction::Same:
    case motion::Direction::Rise:
    case motion::Direction::Fall:
        return true;
    }
    return false;
}

std::expected<void, std::string> validate(
    const Descriptor& descriptor) {
    if (!complete(descriptor.identity) ||
        !complete(descriptor.tonic) ||
        descriptor.roles.empty() ||
        descriptor.ghammaz.empty() ||
        descriptor.regions.empty() ||
        !descriptor.roles.contains(descriptor.tonic) ||
        !subset(descriptor.ghammaz, descriptor.roles) ||
        !subset(descriptor.emphasis, descriptor.roles) ||
        !subset(descriptor.characteristic, descriptor.regions)) {
        return fail("jins descriptor has invalid core role or register sets");
    }
    for (const auto& role : descriptor.roles) {
        if (!complete(role)) {
            return fail("jins descriptor role identity is incomplete");
        }
    }
    for (const auto& region : descriptor.regions) {
        if (!complete(region)) {
            return fail("jins descriptor register identity is incomplete");
        }
    }
    for (const auto& [identity, definition] : descriptor.gestures) {
        if (identity != definition.identity ||
            !complete(identity) ||
            definition.steps.empty()) {
            return fail("jins gesture identity or steps are invalid");
        }
        for (const auto& step : definition.steps) {
            if (step.roles.empty() ||
                step.regions.empty() ||
                step.directions.empty() ||
                !subset(step.roles, descriptor.roles) ||
                !subset(step.regions, descriptor.regions) ||
                !std::ranges::all_of(step.directions, valid)) {
                return fail("jins gesture step domain is invalid");
            }
        }
    }
    auto validReference = [&](const std::set<Identity>& refs) {
        return std::ranges::all_of(refs, [&](const auto& ref) {
            return descriptor.gestures.contains(ref);
        });
    };
    if (!validReference(descriptor.entry) ||
        !validReference(descriptor.exit) ||
        !validReference(descriptor.cadences) ||
        !validReference(descriptor.motifs)) {
        return fail("jins formula references an unknown gesture");
    }
    for (const auto& [identity, baggage] : descriptor.baggage) {
        if (identity != baggage.identity ||
            !complete(identity) ||
            !descriptor.roles.contains(baggage.role) ||
            baggage.regions.empty() ||
            baggage.directions.empty() ||
            !subset(baggage.regions, descriptor.regions) ||
            !std::ranges::all_of(baggage.directions, valid) ||
            !std::ranges::all_of(
                baggage.gestures,
                [&](const auto& gesture) {
                    return descriptor.gestures.contains(gesture);
                })) {
            return fail("jins baggage capability is invalid");
        }
    }
    for (const auto& [gesture, definition] : descriptor.gestures) {
        for (const auto& step : definition.steps) {
            if (!step.baggage) {
                continue;
            }
            const auto found = descriptor.baggage.find(*step.baggage);
            if (found == descriptor.baggage.end() ||
                !step.roles.contains(found->second.role) ||
                !std::ranges::any_of(
                    step.regions,
                    [&](const auto& region) {
                        return found->second.regions.contains(region);
                    }) ||
                !std::ranges::any_of(
                    step.directions,
                    [&](const auto direction) {
                        return found->second.directions.contains(direction);
                    }) ||
                (!found->second.gestures.empty() &&
                 !found->second.gestures.contains(gesture))) {
                return fail(
                    "jins gesture baggage step cannot match capability");
            }
        }
    }
    return {};
}

} // namespace mq::kernel::jins::detail
