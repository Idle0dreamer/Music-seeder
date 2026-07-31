#include "Run.hpp"

#include <algorithm>

#include "mq/kernel/grammar/Catalog.hpp"

namespace mq::kernel::grammar::detail {
namespace {

Diagnostic failure(
    const Term& term,
    const Frame& frame,
    std::string message) {
    return {
        term.identity(),
        frame.scope,
        std::move(message),
        std::nullopt,
    };
}

} // namespace

Batch Runner::stage(
    const Term& term,
    const Stage& form,
    Frame frame) const {
    const auto start = frame.outcome.program.size();
    const auto nested = frame.outcome.stages.size();
    auto result = run(form.body, std::move(frame));
    Batch output;
    output.diagnostics = std::move(result.diagnostics);
    for (auto& item : result.frames) {
        if (item.outcome.stages.size() != nested) {
            output.diagnostics.push_back(
                failure(term, item, "generation stages may not nest"));
            continue;
        }
        if (item.outcome.program.size() <= start) {
            output.diagnostics.push_back(
                failure(term, item, "generation stage is empty"));
            continue;
        }
        std::vector<operation::Any> actions(
            item.outcome.program.begin() +
                static_cast<std::ptrdiff_t>(start),
            item.outcome.program.end());
        item.outcome.stages.push_back({
            form.identity,
            std::move(actions),
        });
        output.frames.push_back(std::move(item));
    }
    return output;
}

Batch Runner::candidate(
    const Term& term,
    const Candidate& form,
    Frame frame) const {
    auto result = run(form.body, std::move(frame));
    Batch output;
    output.diagnostics = std::move(result.diagnostics);
    for (auto& item : result.frames) {
        if (item.outcome.candidate) {
            output.diagnostics.push_back(
                failure(term, item, "generation candidates may not nest"));
            continue;
        }
        item.outcome.candidate = form.identity;
        output.frames.push_back(std::move(item));
    }
    return output;
}

Batch Runner::produce(
    const Term& term,
    const Produce& form,
    Frame frame) const {
    if (!context_.grammar.catalog) {
        return Batch{
            {},
            {failure(
                term,
                frame,
                "missing grammar catalog for recursive production")},
        };
    }

    const auto* production_term = context_.grammar.catalog->find(form.production);
    if (!production_term) {
        return Batch{
            {},
            {failure(
                term,
                frame,
                "grammar production not found: " + form.production.str())},
        };
    }

    auto budget_it = frame.outcome.state.grammar.budget.find(form.production);
    if (budget_it == frame.outcome.state.grammar.budget.end() || budget_it->second == 0) {
        return Batch{
            {},
            {failure(
                term,
                frame,
                "grammar production budget exhausted: " + form.production.str())},
        };
    }

    // Deduct one from the budget
    frame.outcome.state.grammar.budget[form.production] -= 1;

    // Evaluate the production term
    return run(*production_term, std::move(frame));
}

} // namespace mq::kernel::grammar::detail
