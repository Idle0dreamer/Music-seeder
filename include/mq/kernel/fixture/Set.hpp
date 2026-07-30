#pragma once

#include "mq/kernel/path/Graph.hpp"
#include "mq/kernel/jins/Catalog.hpp"
#include "mq/kernel/profile/Set.hpp"
#include "mq/kernel/sayr/Plan.hpp"

#include <expected>
#include <string>

namespace mq::kernel::fixture {

struct Center {
    Identity root;
    Identity branch;
};

struct Jins {
    Identity root;
    Identity branch;
};

struct Role {
    Identity root;
    Identity ghammaz;
    Identity baggage;
};

struct Region {
    Identity root;
    Identity upper;
};

struct Gesture {
    Identity ascent;
    Identity descent;
};

struct Phrase {
    Identity function;
};

struct Regional {
    profile::Set a;
    profile::Set b;
};

struct Profiles {
    profile::Set shared;
    Regional regional;
};

struct Path {
    Identity direct;
    path::Graph graph;
};

struct Sayr {
    struct Obligation {
        Identity establish;
        Identity settle;
        Identity expand;
        Identity climax;
        Identity travel;
        Identity restore;
    } obligation;

    struct Route {
        Identity stay;
        Identity journey;
    } route;

    mq::kernel::sayr::Plan plan;
};

struct Set {
    Center center;
    Jins jins;
    Role role;
    Region region;
    Gesture gesture;
    Identity baggage;
    Phrase phrase;
    Identity cell;
    Identity cadence;
    Path path;
    Sayr sayr;
    mq::kernel::jins::Catalog catalog;
    Profiles profile;
};

[[nodiscard]] std::expected<Set, std::string> make();

} // namespace mq::kernel::fixture
