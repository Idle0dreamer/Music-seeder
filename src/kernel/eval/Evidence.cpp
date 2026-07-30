#include "mq/kernel/eval/Evaluator.hpp"

#include <array>

namespace mq::kernel::eval {
namespace {

const std::array<evidence::Kind, 7> kinds{
    evidence::Kind::Recurrence,
    evidence::Kind::Dwell,
    evidence::Kind::Emphasis,
    evidence::Kind::Cell,
    evidence::Kind::Baggage,
    evidence::Kind::Cadence,
    evidence::Kind::Register,
};

std::string threshold(
    tonicization::Level level,
    evidence::Kind kind) {
    return std::string("threshold.") + tonicization::name(level) + "." +
           evidence::name(kind);
}

} // namespace

std::expected<void, Violation> Evaluator::check(
    const state::Snapshot& state,
    tonicization::Level level,
    std::size_t index,
    const char* operation) const {
    bool found = level == tonicization::Level::Color;
    for (const auto kind : kinds) {
        const auto key = threshold(level, kind);
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
        found = true;
        const auto actual = state.evidence.amount.contains(kind)
                                ? state.evidence.amount.at(kind)
                                : Rational(0);
        if (actual < *required) {
            return std::unexpected(Violation{
                index,
                operation,
                key,
                "insufficient " + std::string(evidence::name(kind)) +
                    " evidence: " + actual.str() + " < " + required->str(),
            });
        }
    }
    if (!found) {
        return std::unexpected(Violation{
            index,
            operation,
            "threshold." + std::string(tonicization::name(level)),
            "profile does not define this tonicization level",
        });
    }
    return {};
}

} // namespace mq::kernel::eval
