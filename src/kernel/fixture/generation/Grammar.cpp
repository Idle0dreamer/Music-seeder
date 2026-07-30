#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

grammar::Term actions(
    const generate::Stage& stage,
    const std::string& prefix) {
    auto result = grammar::Term::atom(
        id(prefix + ".atom.0"),
        stage.actions.front());
    for (std::size_t index = 1; index < stage.actions.size(); ++index) {
        result = grammar::Term::seq(
            id(prefix + ".seq." + std::to_string(index)),
            std::move(result),
            grammar::Term::atom(
                id(prefix + ".atom." + std::to_string(index)),
                stage.actions[index]));
    }
    return result;
}

grammar::Term candidate(
    const generate::Candidate& value,
    const std::string& prefix) {
    const auto& event = value.stages.front();
    return grammar::Term::candidate(
        id(prefix + ".seal"),
        value.identity,
        grammar::Term::stage(
            id(prefix + ".stage"),
            event.identity,
            actions(event, prefix)));
}

} // namespace mq::kernel::fixture::generation::detail
