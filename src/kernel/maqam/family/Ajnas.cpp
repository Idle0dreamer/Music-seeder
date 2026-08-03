#include "Internal.hpp"

#include <algorithm>
#include <map>
#include <optional>
#include <set>
#include <string_view>

namespace mq::kernel::maqam::family::detail {
namespace {

std::expected<Identity, std::string> reference(
    const Key& key,
    std::string_view kind,
    const std::string& name) {
    if (name.empty() || name == "-") {
        return std::unexpected(
            "empty " + std::string(kind) + " reference in " + key.package);
    }
    if (kind == "jins") {
        if (name == "root") {
            return key.jinsRoot;
        }
        const auto branch = std::ranges::find_if(
            key.branches,
            [&](const auto& value) { return value.name == name; });
        if (branch != key.branches.end()) {
            return branch->jins;
        }
    }
    const auto token = name.find('.') == std::string::npos
                           ? std::string(kind) + "." + name
                           : name;
    const auto found = key.authorities.find(token);
    if (found == key.authorities.end()) {
        return std::unexpected(
            "undeclared " + std::string(kind) + " reference " + token +
            " in " + key.package);
    }
    return found->second;
}

std::expected<std::set<Identity>, std::string> references(
    const Key& key,
    std::string_view kind,
    const std::vector<std::string>& names) {
    std::set<Identity> result;
    for (const auto& name : names) {
        const auto value = reference(key, kind, name);
        if (!value) return std::unexpected(value.error());
        result.insert(*value);
    }
    return result;
}

std::expected<const JinsSpec*, std::string> descriptorSpec(
    const Key& key,
    const Identity& descriptor) {
    const auto name = descriptor == key.jinsRoot
                          ? std::string("root")
                          : [&] {
                                const auto branch = std::ranges::find_if(
                                    key.branches,
                                    [&](const auto& value) {
                                        return value.jins == descriptor;
                                    });
                                return branch == key.branches.end()
                                           ? std::string{}
                                           : branch->name;
                            }();
    const auto found = std::ranges::find_if(
        key.jins,
        [&](const auto& value) { return value.name == name; });
    if (found == key.jins.end()) {
        return std::unexpected(
            "missing collection jins descriptor for " + descriptor.str());
    }
    return &*found;
}

std::expected<std::map<Identity, gesture::Definition>, std::string> gestures(
    const Key& key,
    const std::string& owner) {
    std::map<Identity, gesture::Definition> result;
    for (const auto& specification : key.gestures) {
        if (specification.owner != owner) continue;
        const auto identity = reference(key, "gesture", specification.name);
        if (!identity) return std::unexpected(identity.error());
        gesture::Definition definition{*identity, {}};
        for (const auto& source : specification.steps) {
            const auto roles = references(key, "role", source.roles);
            const auto regions = references(key, "region", source.regions);
            if (!roles) return std::unexpected(roles.error());
            if (!regions) return std::unexpected(regions.error());
            std::optional<Identity> baggage;
            if (source.baggage) {
                const auto value = reference(key, "baggage", *source.baggage);
                if (!value) return std::unexpected(value.error());
                baggage = *value;
            }
            definition.steps.push_back({
                *roles,
                *regions,
                {source.direction},
                baggage});
        }
        result.emplace(*identity, std::move(definition));
    }
    return result;
}

std::expected<std::map<Identity, jins::Baggage>, std::string> baggage(
    const Key& key,
    const std::string& owner) {
    std::map<Identity, jins::Baggage> result;
    for (const auto& specification : key.baggage) {
        if (specification.owner != owner) continue;
        const auto identity = reference(key, "baggage", specification.name);
        const auto role = reference(key, "role", specification.role);
        const auto regions = references(key, "region", specification.regions);
        const auto gestures = references(key, "gesture", specification.gestures);
        if (!identity) return std::unexpected(identity.error());
        if (!role) return std::unexpected(role.error());
        if (!regions) return std::unexpected(regions.error());
        if (!gestures) return std::unexpected(gestures.error());
        std::set<motion::Direction> directions(
            specification.directions.begin(), specification.directions.end());
        result.emplace(
            *identity,
            jins::Baggage{
                *identity,
                *role,
                *regions,
                std::move(directions),
                *gestures});
    }
    return result;
}

std::expected<jins::Descriptor, std::string> descriptor(
    const Key& key,
    const Identity& identity) {
    const auto specification = descriptorSpec(key, identity);
    if (!specification) return std::unexpected(specification.error());
    const auto& source = **specification;
    const auto tonic = reference(key, "role", source.tonic);
    const auto roles = references(key, "role", source.roles);
    const auto ghammaz = references(key, "role", source.ghammaz);
    const auto regions = references(key, "region", source.regions);
    const auto characteristic = references(key, "region", source.characteristic);
    const auto emphasis = references(key, "role", source.emphasis);
    const auto entry = references(key, "gesture", source.entry);
    const auto exit = references(key, "gesture", source.exit);
    const auto cadences = references(key, "gesture", source.cadences);
    const auto motifs = references(key, "gesture", source.motifs);
    const auto gestureDefinitions = gestures(key, source.name);
    const auto baggageDefinitions = baggage(key, source.name);
    if (!tonic) return std::unexpected(tonic.error());
    if (!roles) return std::unexpected(roles.error());
    if (!ghammaz) return std::unexpected(ghammaz.error());
    if (!regions) return std::unexpected(regions.error());
    if (!characteristic) return std::unexpected(characteristic.error());
    if (!emphasis) return std::unexpected(emphasis.error());
    if (!entry) return std::unexpected(entry.error());
    if (!exit) return std::unexpected(exit.error());
    if (!cadences) return std::unexpected(cadences.error());
    if (!motifs) return std::unexpected(motifs.error());
    if (!gestureDefinitions) return std::unexpected(gestureDefinitions.error());
    if (!baggageDefinitions) return std::unexpected(baggageDefinitions.error());
    return jins::Descriptor{
        identity,
        *tonic,
        *roles,
        *ghammaz,
        *regions,
        *characteristic,
        *emphasis,
        *entry,
        *exit,
        *cadences,
        *motifs,
        *baggageDefinitions,
        *gestureDefinitions,
        {key.source, "family:" + key.family, "jins:" + source.name},
    };
}

} // namespace

std::expected<jins::Catalog, std::string> ajnas(const Key& key) {
    if (key.jins.empty()) {
        return std::unexpected(
            "family package has no collection jins descriptors: " +
            key.package);
    }
    jins::Catalog result;
    const auto root = descriptor(key, key.jinsRoot);
    if (!root) return std::unexpected(root.error());
    auto added = result.add(*root);
    if (!added) return std::unexpected(added.error());
    for (const auto& branch : key.branches) {
        const auto upper = descriptor(key, branch.jins);
        if (!upper) return std::unexpected(upper.error());
        added = result.add(*upper);
        if (!added) return std::unexpected(added.error());
    }
    return result;
}

} // namespace mq::kernel::maqam::family::detail
