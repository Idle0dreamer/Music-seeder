#pragma once

#include "mq/kernel/Operator.hpp"
#include "mq/kernel/Profile.hpp"

#include <expected>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace mq::kernel {

struct TraceEvent {
    std::size_t index{};
    std::string operation;
    std::string subject;
};

struct State {
    std::vector<Identity> centerStack;
    std::optional<Identity> activeJins;
    TonicizationLevel level{TonicizationLevel::Color};
    std::map<EvidenceKind, Rational> evidence;
    std::map<Identity, std::size_t> cellOccurrences;
    std::vector<TraceEvent> trace;
};

struct Violation {
    std::size_t operationIndex{};
    std::string operation;
    std::string rule;
    std::string message;
};

class Evaluator {
public:
    explicit Evaluator(const Profile& profile);

    [[nodiscard]] std::expected<State, Violation> run(
        State state,
        std::span<const Operation> program) const;

private:
    const Profile& profile_;

    [[nodiscard]] std::expected<State, Violation> apply(
        State state,
        const Operation& operation,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> requireEvidence(
        const State& state,
        TonicizationLevel level,
        std::size_t index,
        const char* operation) const;
};

} // namespace mq::kernel

