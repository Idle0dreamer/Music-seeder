#include "Node.hpp"

#include <set>

namespace mq::kernel::grammar {

Term::Term(std::shared_ptr<const detail::Node> node)
    : node_(std::move(node)) {}

Term Term::id(Identity identity) {
    return Term(std::make_shared<detail::Node>(
        detail::Node{std::move(identity), detail::Id{}}));
}

Term Term::fail(Identity identity, std::string message) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Fail{std::move(message)},
    }));
}

Term Term::atom(Identity identity, operation::Any operation) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Atom{std::move(operation)},
    }));
}

Term Term::seq(Identity identity, Term first, Term second) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Seq{std::move(first), std::move(second)},
    }));
}

std::expected<Term, std::string> Term::alt(
    Identity identity,
    std::vector<Branch> branches) {
    if (branches.empty()) {
        return std::unexpected("Alt requires at least one branch");
    }
    std::set<Identity> identities;
    for (const auto& branch : branches) {
        if (!identities.insert(branch.identity).second) {
            return std::unexpected(
                "Alt branch identity is duplicated: " + branch.identity.str());
        }
    }
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Alt{std::move(branches)},
    }));
}

Term Term::guard(
    Identity identity,
    guard::Predicate predicate,
    Term body) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Guard{std::move(predicate), std::move(body)},
    }));
}

std::expected<Term, std::string> Term::repeat(
    Identity identity,
    repeat::Bounds bounds,
    Term body) {
    if (bounds.minimum > bounds.maximum) {
        return std::unexpected("Repeat minimum exceeds maximum");
    }
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Repeat{bounds, std::move(body)},
    }));
}

Term Term::scope(
    Identity identity,
    scope::Policy policy,
    Term body) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Scope{std::move(policy), std::move(body)},
    }));
}

Term Term::bind(Identity identity, Identity binding, Term body) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Bind{std::move(binding), std::move(body)},
    }));
}

Term Term::stage(
    Identity identity,
    Identity stage,
    Term body) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Stage{std::move(stage), std::move(body)},
    }));
}

Term Term::candidate(
    Identity identity,
    Identity candidate,
    Term body) {
    return Term(std::make_shared<detail::Node>(detail::Node{
        std::move(identity),
        detail::Candidate{std::move(candidate), std::move(body)},
    }));
}

const Identity& Term::identity() const noexcept {
    return node_->identity;
}

} // namespace mq::kernel::grammar
