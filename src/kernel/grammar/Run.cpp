#include "Run.hpp"

#include "mq/kernel/eval/Evaluator.hpp"

#include <array>
#include <type_traits>

namespace mq::kernel::grammar::detail {

Runner::Runner(const profile::Set& profile)
    : profile_(profile) {}

Runner::Runner(const profile::Set& profile, const path::Graph& paths)
    : profile_(profile),
      paths_(&paths) {}

void append(Batch& target, Batch source) {
    target.frames.insert(
        target.frames.end(),
        std::make_move_iterator(source.frames.begin()),
        std::make_move_iterator(source.frames.end()));
    target.diagnostics.insert(
        target.diagnostics.end(),
        std::make_move_iterator(source.diagnostics.begin()),
        std::make_move_iterator(source.diagnostics.end()));
}

Batch Runner::run(const Term& term, Frame frame) const {
    return std::visit(
        [&](const auto& form) -> Batch {
            using T = std::decay_t<decltype(form)>;
            if constexpr (std::is_same_v<T, Id>) {
                return Batch{{std::move(frame)}, {}};
            } else if constexpr (std::is_same_v<T, Fail>) {
                return Batch{
                    {},
                    {{
                        term.identity(),
                        frame.scope,
                        form.message,
                        std::nullopt,
                    }},
                };
            } else if constexpr (std::is_same_v<T, Atom>) {
                const std::array program{form.operation};
                auto result = paths_ == nullptr
                                ? eval::Evaluator(profile_).run(
                                      frame.outcome.state,
                                      program)
                                : eval::Evaluator(profile_, *paths_).run(
                                      frame.outcome.state,
                                      program);
                if (!result) {
                    return Batch{
                        {},
                        {{
                            term.identity(),
                            frame.scope,
                            result.error().message,
                            result.error(),
                        }},
                    };
                }
                frame.outcome.state = std::move(*result);
                return Batch{{std::move(frame)}, {}};
            } else if constexpr (std::is_same_v<T, Seq>) {
                return seq(term, form, std::move(frame));
            } else if constexpr (std::is_same_v<T, Alt>) {
                return alt(term, form, std::move(frame));
            } else if constexpr (std::is_same_v<T, Repeat>) {
                return repeat(term, form, std::move(frame));
            } else if constexpr (std::is_same_v<T, Scope>) {
                return scope(term, form, std::move(frame));
            } else if constexpr (std::is_same_v<T, Guard>) {
                if (!guard::holds(
                        form.predicate,
                        frame.outcome.state,
                        frame.bindings)) {
                    return Batch{
                        {},
                        {{
                            term.identity(),
                            frame.scope,
                            "guard rejected branch",
                            std::nullopt,
                        }},
                    };
                }
                return run(form.body, std::move(frame));
            } else {
                const auto bindings = frame.bindings;
                frame.bindings.insert(form.binding);
                auto result = run(form.body, std::move(frame));
                for (auto& output : result.frames) {
                    output.bindings = bindings;
                }
                return result;
            }
        },
        term.node_->form);
}

} // namespace mq::kernel::grammar::detail
