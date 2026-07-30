#include "mq/kernel/Evaluator.hpp"

#include <algorithm>
#include <array>
#include <type_traits>

namespace mq::kernel {
namespace {

const std::array<EvidenceKind, 7> evidenceKinds{
    EvidenceKind::Recurrence,
    EvidenceKind::Dwell,
    EvidenceKind::Emphasis,
    EvidenceKind::Cell,
    EvidenceKind::Baggage,
    EvidenceKind::Cadence,
    EvidenceKind::Register,
};

std::string thresholdKey(
    TonicizationLevel level,
    EvidenceKind kind) {
    return std::string("threshold.") + name(level) + "." + name(kind);
}

Violation denied(
    std::size_t index,
    const char* operation,
    std::string rule,
    std::string subject) {
    return Violation{
        index,
        operation,
        std::move(rule),
        "profile rejected " + std::move(subject),
    };
}

} // namespace

Evaluator::Evaluator(const Profile& profile)
    : profile_(profile) {}

std::expected<State, Violation> Evaluator::run(
    State state,
    std::span<const Operation> program) const {
    for (std::size_t index = 0; index < program.size(); ++index) {
        auto next = apply(std::move(state), program[index], index);
        if (!next) {
            return std::unexpected(next.error());
        }
        state = std::move(*next);
    }
    return state;
}

std::expected<void, Violation> Evaluator::requireEvidence(
    const State& state,
    TonicizationLevel level,
    std::size_t index,
    const char* operation) const {
    bool foundThreshold = level == TonicizationLevel::Color;
    for (const auto kind : evidenceKinds) {
        const auto key = thresholdKey(level, kind);
        const auto* rule = profile_.find(key);
        if (rule == nullptr) {
            continue;
        }
        const auto* required = std::get_if<Rational>(&rule->value);
        if (required == nullptr) {
            return std::unexpected(Violation{
                index,
                operation,
                key,
                "evidence threshold is not rational",
            });
        }
        foundThreshold = true;
        const auto actual = state.evidence.contains(kind)
                                ? state.evidence.at(kind)
                                : Rational(0);
        if (actual < *required) {
            return std::unexpected(Violation{
                index,
                operation,
                key,
                "insufficient " + std::string(name(kind)) +
                    " evidence: " + actual.str() + " < " + required->str(),
            });
        }
    }
    if (!foundThreshold) {
        return std::unexpected(Violation{
            index,
            operation,
            "threshold." + std::string(name(level)),
            "profile does not define this tonicization level",
        });
    }
    return {};
}

std::expected<State, Violation> Evaluator::apply(
    State state,
    const Operation& operation,
    std::size_t index) const {
    const auto operationName = name(operation);
    std::string subject;

    const auto outcome = std::visit(
        [&](const auto& value) -> std::expected<void, Violation> {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, Anchor>) {
                subject = value.center.str();
                if (!profile_.allows("allow.anchor", value.center)) {
                    return std::unexpected(
                        denied(index, operationName, "allow.anchor", subject));
                }
                if (state.centerStack.empty() ||
                    state.centerStack.back() != value.center) {
                    state.centerStack.push_back(value.center);
                }
            } else if constexpr (std::is_same_v<T, Enter>) {
                subject = value.jins.str();
                if (!profile_.allows("allow.enter", value.jins)) {
                    return std::unexpected(
                        denied(index, operationName, "allow.enter", subject));
                }
                state.activeJins = value.jins;
            } else if constexpr (std::is_same_v<T, Emphasize>) {
                subject = value.role.str();
                if (!profile_.allows("allow.emphasize", value.role)) {
                    return std::unexpected(denied(
                        index,
                        operationName,
                        "allow.emphasize",
                        subject));
                }
                state.evidence[EvidenceKind::Emphasis] += value.amount;
            } else if constexpr (std::is_same_v<T, Dwell>) {
                subject = value.role.str();
                if (!profile_.allows("allow.dwell", value.role)) {
                    return std::unexpected(
                        denied(index, operationName, "allow.dwell", subject));
                }
                state.evidence[EvidenceKind::Dwell] += value.amount;
            } else if constexpr (std::is_same_v<T, Emit>) {
                subject = value.cell.str();
                if (!profile_.allows("allow.emit", value.cell)) {
                    return std::unexpected(
                        denied(index, operationName, "allow.emit", subject));
                }
                const auto previous = state.cellOccurrences[value.cell]++;
                state.evidence[EvidenceKind::Cell] += Rational(1);
                if (previous > 0) {
                    state.evidence[EvidenceKind::Recurrence] += Rational(1);
                }
            } else if constexpr (std::is_same_v<T, Cadence>) {
                subject = value.family.str();
                if (!profile_.allows("allow.cadence", value.family)) {
                    return std::unexpected(
                        denied(index, operationName, "allow.cadence", subject));
                }
                state.evidence[EvidenceKind::Cadence] += value.evidence;
            } else if constexpr (std::is_same_v<T, Tonicize>) {
                subject = value.jins.str();
                if (!profile_.allows("allow.tonicize", value.jins)) {
                    return std::unexpected(denied(
                        index,
                        operationName,
                        "allow.tonicize",
                        subject));
                }
                const auto evidence =
                    requireEvidence(state, value.level, index, operationName);
                if (!evidence) {
                    return evidence;
                }
                state.activeJins = value.jins;
                state.level = value.level;
            } else if constexpr (std::is_same_v<T, Modulate>) {
                subject = value.path.str();
                if (!profile_.allows("allow.modulate", value.path)) {
                    return std::unexpected(denied(
                        index,
                        operationName,
                        "allow.modulate",
                        subject));
                }
                const auto evidence =
                    requireEvidence(state, value.level, index, operationName);
                if (!evidence) {
                    return evidence;
                }
                state.centerStack.push_back(value.targetCenter);
                state.level = value.level;
            } else if constexpr (std::is_same_v<T, Return>) {
                subject = value.center.str();
                const auto found =
                    std::ranges::find(state.centerStack, value.center);
                if (found == state.centerStack.end()) {
                    return std::unexpected(Violation{
                        index,
                        operationName,
                        "center.stack",
                        "return target was never established: " + subject,
                    });
                }
                state.centerStack.erase(found + 1, state.centerStack.end());
                state.level = TonicizationLevel::Internal;
            }
            return {};
        },
        operation);

    if (!outcome) {
        return std::unexpected(outcome.error());
    }
    state.trace.push_back(TraceEvent{index, operationName, std::move(subject)});
    return state;
}

} // namespace mq::kernel

