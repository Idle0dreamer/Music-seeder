#include "Run.hpp"

namespace mq::kernel::grammar::detail {

Batch Runner::seq(
    const Term& unused,
    const Seq& form,
    Frame frame) const {
    static_cast<void>(unused);
    auto first = run(form.first, std::move(frame));
    Batch result;
    result.diagnostics = std::move(first.diagnostics);
    for (auto& outcome : first.frames) {
        append(result, run(form.second, std::move(outcome)));
    }
    return result;
}

Batch Runner::alt(
    const Term& term,
    const Alt& form,
    Frame frame) const {
    Batch result;
    for (const auto& branch : form.branches) {
        auto input = frame;
        const auto cost = choice::sum(input.outcome.cost, branch.cost);
        if (!cost) {
            result.diagnostics.push_back({
                term.identity(),
                input.scope,
                cost.error(),
                std::nullopt,
            });
            continue;
        }
        input.outcome.cost = *cost;
        input.outcome.decisions.push_back(choice::Draw{
            term.identity(),
            input.scope,
            branch.identity,
        });
        append(result, run(branch.body, std::move(input)));
    }
    return result;
}

Batch Runner::repeat(
    const Term& term,
    const Repeat& form,
    Frame frame) const {
    Batch result;
    for (auto count = form.bounds.minimum;
         count <= form.bounds.maximum;
         ++count) {
        auto input = frame;
        input.outcome.decisions.push_back(choice::Draw{
            term.identity(),
            input.scope,
            Identity{"grammar.repeat", std::to_string(count), "1"},
        });
        Batch current{{std::move(input)}, {}};
        for (std::size_t iteration = 0; iteration < count; ++iteration) {
            Batch next;
            next.diagnostics = std::move(current.diagnostics);
            for (auto& outcome : current.frames) {
                append(next, run(form.body, std::move(outcome)));
            }
            current = std::move(next);
        }
        append(result, std::move(current));
        if (count == form.bounds.maximum) {
            break;
        }
    }
    return result;
}

} // namespace mq::kernel::grammar::detail
