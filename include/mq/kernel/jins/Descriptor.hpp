#pragma once

#include "mq/kernel/gesture/Definition.hpp"
#include "mq/kernel/jins/Baggage.hpp"

#include <map>
#include <set>

namespace mq::kernel::jins {

struct Descriptor {
    Identity identity;
    Identity tonic;
    std::set<Identity> roles;
    std::set<Identity> ghammaz;
    std::set<Identity> regions;
    std::set<Identity> characteristic;
    std::set<Identity> emphasis;
    std::set<Identity> entry;
    std::set<Identity> exit;
    std::set<Identity> cadences;
    std::set<Identity> motifs;
    std::map<Identity, Baggage> baggage;
    std::map<Identity, gesture::Definition> gestures;
    std::vector<std::string> provenance;

    bool operator==(const Descriptor&) const = default;
};

} // namespace mq::kernel::jins
