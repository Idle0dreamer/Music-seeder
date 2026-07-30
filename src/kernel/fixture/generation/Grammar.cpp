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
    auto body = grammar::Term::stage(
        id(prefix + ".stage.0"),
        value.stages.front().identity,
        actions(value.stages.front(), prefix + ".stage.0"));
    for (std::size_t index = 1; index < value.stages.size(); ++index) {
        body = grammar::Term::seq(
            id(prefix + ".stage.seq." + std::to_string(index)),
            std::move(body),
            grammar::Term::stage(
                id(prefix + ".stage." + std::to_string(index)),
                value.stages[index].identity,
                actions(
                    value.stages[index],
                    prefix + ".stage." + std::to_string(index))));
    }
    return grammar::Term::candidate(
        id(prefix + ".seal"),
        value.identity,
        std::move(body));
}

} // namespace mq::kernel::fixture::generation::detail
