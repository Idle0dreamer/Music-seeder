#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/pitch/field/Schema.hpp"
#include "mq/kernel/pitch/field/project/Plan.hpp"
#include "mq/kernel/grammar/Term.hpp"
#include "mq/kernel/jins/Catalog.hpp"
#include "mq/kernel/path/Graph.hpp"
#include "mq/kernel/profile/Set.hpp"
#include "mq/kernel/sayr/Plan.hpp"

#include <expected>
#include <memory>
#include <string>

namespace mq::kernel::maqam {

struct Generation {
    Identity choice;
    grammar::Term production;
    pitch::field::project::Plan projection;
    pitch::field::Schema schema;
};

struct Scaffold {
    jins::Catalog ajnas;
    path::Graph graph;
    sayr::Plan sayr;
    std::shared_ptr<profile::Set> profile;
    Generation generation;
};

std::expected<Scaffold, std::string> make_bayati();

} // namespace mq::kernel::maqam
