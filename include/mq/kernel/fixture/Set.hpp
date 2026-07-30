#pragma once

#include "mq/kernel/path/Graph.hpp"
#include "mq/kernel/profile/Set.hpp"

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

struct Set {
    Center center;
    Jins jins;
    Role role;
    Phrase phrase;
    Identity cell;
    Identity cadence;
    Path path;
    Profiles profile;
};

[[nodiscard]] std::expected<Set, std::string> make();

} // namespace mq::kernel::fixture
