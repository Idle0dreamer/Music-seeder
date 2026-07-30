#pragma once

#include "mq/kernel/eval/Context.hpp"
#include "mq/kernel/operation/Operation.hpp"
#include "mq/kernel/profile/Set.hpp"
#include "mq/kernel/state/Snapshot.hpp"

#include <expected>
#include <span>
#include <string>

namespace mq::kernel::eval {

struct Violation {
    std::size_t index{};
    std::string operation;
    std::string rule;
    std::string message;
};

class Evaluator {
public:
    explicit Evaluator(const profile::Set& profile);
    Evaluator(const profile::Set& profile, Context context);
    Evaluator(const profile::Set& profile, const path::Graph& paths);
    Evaluator(const profile::Set& profile, const jins::Catalog& catalog);
    Evaluator(
        const profile::Set& profile,
        const jins::Catalog& catalog,
        const path::Graph& paths);

    [[nodiscard]] std::expected<state::Snapshot, Violation> run(
        state::Snapshot state,
        std::span<const operation::Any> program) const;

private:
    const profile::Set& profile_;
    Context context_;

    [[nodiscard]] std::expected<state::Snapshot, Violation> apply(
        state::Snapshot state,
        const operation::Any& operation,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> check(
        const state::Snapshot& state,
        tonicization::Level level,
        std::size_t index,
        const char* operation) const;
    [[nodiscard]] std::expected<void, Violation> check(
        const state::Snapshot& state,
        const operation::Modulate& modulation,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> place(
        state::Snapshot& state,
        const operation::Place& event,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> enter(
        state::Snapshot& state,
        const operation::Enter& action,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> emphasize(
        state::Snapshot& state,
        const operation::Emphasize& action,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> dwell(
        state::Snapshot& state,
        const operation::Dwell& action,
        std::size_t index) const;
    [[nodiscard]] std::expected<const jins::Descriptor*, Violation> descriptor(
        const state::Snapshot& state,
        std::size_t index,
        const char* operation) const;
    [[nodiscard]] std::expected<const jins::Descriptor*, Violation> descriptor(
        const Identity& identity,
        std::size_t index,
        const char* operation) const;
    [[nodiscard]] std::expected<void, Violation> begin(
        state::Snapshot& state,
        const operation::Begin& phrase,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> cadence(
        state::Snapshot& state,
        const operation::Cadence& cadence,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> end(
        state::Snapshot& state,
        const operation::End& phrase,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> begin(
        state::Snapshot& state,
        const operation::gesture::Begin& gesture,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> end(
        state::Snapshot& state,
        const operation::gesture::End& gesture,
        std::size_t index) const;
    [[nodiscard]] std::expected<void, Violation> fulfill(
        state::Snapshot& state,
        const operation::sayr::Fulfill& action,
        std::size_t index) const;
};

} // namespace mq::kernel::eval
