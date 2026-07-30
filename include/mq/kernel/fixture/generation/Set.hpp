#pragma once

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/generate/Candidate.hpp"
#include "mq/kernel/grammar/Term.hpp"
#include "mq/kernel/pitch/field/Schema.hpp"
#include "mq/kernel/pitch/field/project/Plan.hpp"

#include <expected>

namespace mq::kernel::fixture::generation {

struct Program {
    generate::Candidate stay;
    generate::Candidate travel;
};

struct Set {
    Identity choice;
    Program program;
    grammar::Term production;
    pitch::field::project::Plan projection;
    pitch::field::Schema schema;
};

[[nodiscard]] std::expected<Set, std::string> make(
    const fixture::Set& fixture);

} // namespace mq::kernel::fixture::generation
