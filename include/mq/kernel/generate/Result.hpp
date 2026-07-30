#pragma once

#include "mq/kernel/choice/Select.hpp"
#include "mq/kernel/eval/Evaluator.hpp"
#include "mq/kernel/grammar/Result.hpp"
#include "mq/kernel/performance/Plan.hpp"
#include "mq/kernel/pitch/request/Run.hpp"

#include <optional>

namespace mq::kernel::generate {

struct Outcome {
    Identity candidate;
    choice::Cost cost;
    state::Snapshot state;
    performance::Plan plan;
};

struct Diagnostic {
    Identity candidate;
    std::optional<Identity> stage;
    std::string message;
    std::optional<eval::Violation> evaluation;
    std::optional<pitch::request::Error> pitch;
};

struct Result {
    Identity selected;
    std::vector<Outcome> legal;
    std::vector<Diagnostic> rejected;
    std::vector<grammar::Diagnostic> derivation;
};

struct Error {
    enum class Code {
        Input,
        Count,
        Choice,
        NoLegal,
    };

    Code code;
    std::string message;
    std::vector<Diagnostic> rejected;
    std::vector<grammar::Diagnostic> derivation;
};

} // namespace mq::kernel::generate
