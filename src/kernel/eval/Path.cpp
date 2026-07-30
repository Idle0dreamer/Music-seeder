#include "mq/kernel/eval/Evaluator.hpp"

namespace mq::kernel::eval {

std::expected<void, Violation> Evaluator::check(
    const state::Snapshot& state,
    const operation::Modulate& modulation,
    std::size_t index) const {
    if (paths_ == nullptr) {
        return std::unexpected(Violation{
            index,
            "Modulate",
            "path.graph",
            "no path graph was selected",
        });
    }
    const auto* rule = paths_->find(modulation.path);
    if (rule == nullptr) {
        return std::unexpected(Violation{
            index,
            "Modulate",
            "path.graph",
            "path has no graph rule: " + modulation.path.str(),
        });
    }
    if (state.center.stack.empty() ||
        state.center.stack.back() != rule->source) {
        return std::unexpected(Violation{
            index,
            "Modulate",
            "path.source",
            "active center does not match path source",
        });
    }
    if (modulation.center != rule->target) {
        return std::unexpected(Violation{
            index,
            "Modulate",
            "path.target",
            "requested center does not match path target",
        });
    }
    if (modulation.level > rule->maximum) {
        return std::unexpected(Violation{
            index,
            "Modulate",
            "path.level",
            "requested tonicization exceeds path maximum",
        });
    }
    for (const auto& required : rule->prior) {
        if (!state.path.completed.contains(required)) {
            return std::unexpected(Violation{
                index,
                "Modulate",
                "path.prerequisite",
                "required prior path is missing: " + required.str(),
            });
        }
    }
    return {};
}

} // namespace mq::kernel::eval
