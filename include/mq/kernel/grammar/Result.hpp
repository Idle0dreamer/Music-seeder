#pragma once

#include "mq/kernel/choice/Select.hpp"
#include "mq/kernel/eval/Evaluator.hpp"

#include <optional>
#include <string>
#include <vector>

namespace mq::kernel::grammar {

struct Stage {
    Identity identity;
    std::vector<operation::Any> actions;
};

struct Diagnostic {
    Identity term;
    std::vector<Identity> scope;
    std::string message;
    std::optional<eval::Violation> violation;
};

struct Outcome {
    std::optional<Identity> candidate;
    state::Snapshot state;
    choice::Cost cost;
    std::vector<choice::Draw> decisions;
    std::vector<operation::Any> program;
    std::vector<Stage> stages;
};

struct Result {
    std::vector<Outcome> outcomes;
    std::vector<Diagnostic> diagnostics;
};

} // namespace mq::kernel::grammar
