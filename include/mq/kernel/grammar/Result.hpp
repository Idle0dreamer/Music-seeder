#pragma once

#include "mq/kernel/choice/Select.hpp"
#include "mq/kernel/eval/Evaluator.hpp"

#include <optional>
#include <string>
#include <vector>

namespace mq::kernel::grammar {

struct Diagnostic {
    Identity term;
    std::vector<Identity> scope;
    std::string message;
    std::optional<eval::Violation> violation;
};

struct Outcome {
    state::Snapshot state;
    choice::Cost cost;
    std::vector<choice::Draw> decisions;
};

struct Result {
    std::vector<Outcome> outcomes;
    std::vector<Diagnostic> diagnostics;
};

} // namespace mq::kernel::grammar
