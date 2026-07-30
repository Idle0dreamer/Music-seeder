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
    Runner(const profile::Set& profile, eval::Context context);

    [[nodiscard]] Batch run(const Term& term, Frame frame) const;

private:
    const profile::Set& profile_;
    eval::Context context_;

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
    [[nodiscard]] Batch stage(
        const Term& term,
        const Stage& form,
        Frame frame) const;
    [[nodiscard]] Batch candidate(
        const Term& term,
        const Candidate& form,
        Frame frame) const;
};

void append(Batch& target, Batch source);

} // namespace mq::kernel::grammar::detail
