#pragma once

#include "mq/kernel/grammar/Term.hpp"

#include <variant>

namespace mq::kernel::grammar::detail {

struct Id {};

struct Fail {
    std::string message;
};

struct Atom {
    operation::Any operation;
};

struct Seq {
    Term first;
    Term second;
};

struct Alt {
    std::vector<Branch> branches;
};

struct Guard {
    guard::Predicate predicate;
    Term body;
};

struct Repeat {
    repeat::Bounds bounds;
    Term body;
};

struct Scope {
    scope::Policy policy;
    Term body;
};

struct Bind {
    Identity binding;
    Term body;
};

struct Stage {
    Identity identity;
    Term body;
};

struct Candidate {
    Identity identity;
    Term body;
};

using Form = std::variant<
    Id,
    Fail,
    Atom,
    Seq,
    Alt,
    Guard,
    Repeat,
    Scope,
    Bind,
    Stage,
    Candidate>;

struct Node {
    Identity identity;
    Form form;
};

} // namespace mq::kernel::grammar::detail
