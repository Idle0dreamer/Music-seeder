#pragma once

#include "Node.hpp"
#include "mq/kernel/grammar/Result.hpp"

#include <set>

namespace mq::kernel::grammar::detail {

struct Frame {
    Outcome outcome;
    std::vector<Identity> scope;
    std::set<Identity> bindings;
};

struct Batch {
    std::vector<Frame> frames;
    std::vector<Diagnostic> diagnostics;
};

class Runner {
public:
    explicit Runner(const profile::Set& profile);
    Runner(const profile::Set& profile, const path::Graph& paths);

    [[nodiscard]] Batch run(const Term& term, Frame frame) const;

private:
    const profile::Set& profile_;
    const path::Graph* paths_{};

    [[nodiscard]] Batch seq(
        const Term& term,
        const Seq& form,
        Frame frame) const;
    [[nodiscard]] Batch alt(
        const Term& term,
        const Alt& form,
        Frame frame) const;
    [[nodiscard]] Batch repeat(
        const Term& term,
        const Repeat& form,
        Frame frame) const;
    [[nodiscard]] Batch scope(
        const Term& term,
        const Scope& form,
        Frame frame) const;
};

void append(Batch& target, Batch source);

} // namespace mq::kernel::grammar::detail
