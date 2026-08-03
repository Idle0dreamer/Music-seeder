#include "mq/kernel/eval/Evaluator.hpp"

#include <algorithm>
#include <type_traits>

namespace mq::kernel::eval {
namespace {

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

std::expected<state::Snapshot, Violation> Evaluator::apply(
    state::Snapshot state,
    const operation::Any& action,
    std::size_t index) const {
    const auto label = operation::name(action);
    std::string subject;

    const auto outcome = std::visit(
        [&](const auto& value) -> std::expected<void, Violation> {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, operation::Anchor>) {
                subject = value.center.str();
                if (!profile_.allows("allow.anchor", value.center.identity)) {
                    return std::unexpected(
                        denied(index, label, "allow.anchor", subject));
                }
                if (state.center.stack.empty() ||
                    state.center.stack.back() != value.center) {
                    state.center.stack.push_back(value.center);
                }
            } else if constexpr (std::is_same_v<T, operation::Enter>) {
                subject = value.jins.str();
                return enter(state, value, index);
            } else if constexpr (std::is_same_v<T, operation::Emphasize>) {
                subject = value.role.str();
                return emphasize(state, value, index);
            } else if constexpr (std::is_same_v<T, operation::Dwell>) {
                subject = value.role.str();
                return dwell(state, value, index);
            } else if constexpr (std::is_same_v<T, operation::Emit>) {
                subject = value.cell.str();
                if (!profile_.allows("allow.emit", value.cell.identity)) {
                    return std::unexpected(
                        denied(index, label, "allow.emit", subject));
                }
                if (!state.melody.current) {
                    return std::unexpected(Violation{
                        index,
                        label,
                        "cell.event",
                        "cell emission requires a current structural event",
                    });
                }
                const sort::EventId event{state.melody.current->identity};
                if (state.cell.owners.contains(event)) {
                    return std::unexpected(Violation{
                        index,
                        label,
                        "cell.event",
                        "structural event already belongs to a cell",
                    });
                }
                if (value.formula && !profile_.allows(
                        "allow.variation", value.formula->identity)) {
                    return std::unexpected(denied(
                        index,
                        label,
                        "allow.variation",
                        value.formula->str()));
                }
                if (value.variation &&
                    !profile_.allows(
                        "allow.variation", value.variation->identity)) {
                    return std::unexpected(denied(
                        index,
                        label,
                        "allow.variation",
                        value.variation->str()));
                }
                if (value.motif &&
                    !profile_.allows("allow.motif", value.motif->identity)) {
                    return std::unexpected(denied(
                        index, label, "allow.motif", value.motif->str()));
                }
                if (value.variation) {
                    if (!value.formula) {
                        return std::unexpected(Violation{
                            index,
                            label,
                            "cell.variation",
                            "a variation requires a declared base formula",
                        });
                    }
                    const auto has_base = std::ranges::any_of(
                        state.cell.owners,
                        [&](const auto& entry) {
                            return entry.second.formula &&
                                   entry.second.formula->identity ==
                                       value.formula->identity;
                        });
                    if (!has_base) {
                        return std::unexpected(Violation{
                            index,
                            label,
                            "cell.variation",
                            "a variation requires a prior occurrence of its "
                            "base formula",
                        });
                    }
                }
                state.cell.owners.emplace(
                    event,
                    state::Cell::Owner{
                        value.cell,
                        value.formula,
                        value.variation,
                        value.motif});
                if (value.motif) {
                    state.motif.occurrences[*value.motif].push_back(
                        state::MotifOccurrence{
                            event,
                            value.formula,
                            value.variation});
                }
                const auto previous = state.cell.occurrences[value.cell]++;
                state.evidence.amount[evidence::Kind::Cell] += Rational(1);
                if (previous > 0) {
                    state.evidence.amount[evidence::Kind::Recurrence] +=
                        Rational(1);
                }
            } else if constexpr (std::is_same_v<T, operation::Cadence>) {
                subject = value.family.str();
                return cadence(state, value, index);
            } else if constexpr (std::is_same_v<T, operation::Tonicize>) {
                subject = value.jins.str();
                if (state.gesture.active) {
                    return std::unexpected(Violation{
                        index,
                        label,
                        "gesture.active",
                        "active gesture must end before tonicization",
                    });
                }
                if (!profile_.allows("allow.tonicize", value.jins.identity)) {
                    return std::unexpected(
                        denied(index, label, "allow.tonicize", subject));
                }
                const auto evidence = check(state, value.level, index, label);
                if (!evidence) {
                    return evidence;
                }
                const auto selected = descriptor(value.jins.identity, index, label);
                if (!selected) {
                    return std::unexpected(selected.error());
                }
                state.jins.active = sort::JinsId{(*selected)->identity};
                state.tonicization.level = value.level;
            } else if constexpr (std::is_same_v<T, operation::Modulate>) {
                subject = value.path.str();
                if (!profile_.allows("allow.modulate", value.path.identity)) {
                    return std::unexpected(
                        denied(index, label, "allow.modulate", subject));
                }
                const auto path = check(state, value, index);
                if (!path) {
                    return path;
                }
                const auto evidence = check(state, value.level, index, label);
                if (!evidence) {
                    return evidence;
                }
                state.center.stack.push_back(value.center);
                state.path.completed.insert(value.path);
                state.tonicization.level = value.level;
            } else if constexpr (std::is_same_v<T, operation::Place>) {
                subject = value.event.str();
                return place(state, value, index);
            } else if constexpr (std::is_same_v<T, operation::Begin>) {
                subject = value.phrase.str();
                return begin(state, value, index);
            } else if constexpr (std::is_same_v<T, operation::End>) {
                subject = value.phrase.str();
                return end(state, value, index);
            } else if constexpr (
                std::is_same_v<T, operation::gesture::Begin>) {
                subject = value.occurrence.str();
                return begin(state, value, index);
            } else if constexpr (
                std::is_same_v<T, operation::gesture::End>) {
                subject = value.occurrence.str();
                return end(state, value, index);
            } else if constexpr (
                std::is_same_v<T, operation::sayr::Fulfill>) {
                subject = value.obligation.str();
                return fulfill(state, value, index);
            } else if constexpr (std::is_same_v<T, operation::Return>) {
                subject = value.center.str();
                const auto found =
                    std::ranges::find(state.center.stack, value.center);
                if (found == state.center.stack.end()) {
                    return std::unexpected(Violation{
                        index,
                        label,
                        "center.stack",
                        "return target was never established: " + subject,
                    });
                }
                state.center.stack.erase(found + 1, state.center.stack.end());
                state.tonicization.level = tonicization::Level::Internal;
            }
            return {};
        },
        action);

    if (!outcome) {
        return std::unexpected(outcome.error());
    }
    state.trace.events.push_back(
        trace::Event{index, label, std::move(subject)});
    return state;
}

} // namespace mq::kernel::eval
