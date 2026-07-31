#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/choice/Select.hpp"
#include "mq/kernel/grammar/Guard.hpp"
#include "mq/kernel/grammar/Scope.hpp"
#include "mq/kernel/operation/Operation.hpp"

#include <expected>
#include <memory>
#include <string>
#include <vector>

namespace mq::kernel::grammar {

namespace detail {
struct Node;
class Runner;
} // namespace detail

struct Branch;

namespace repeat {

struct Bounds {
    std::size_t minimum{};
    std::size_t maximum{};
};

} // namespace repeat

class Term {
public:
    static Term id(Identity identity);
    static Term fail(Identity identity, std::string message);
    static Term atom(Identity identity, operation::Any operation);
    static Term seq(Identity identity, Term first, Term second);
    static std::expected<Term, std::string> alt(
        Identity identity,
        std::vector<Branch> branches);
    static Term guard(
        Identity identity,
        guard::Predicate predicate,
        Term body);
    static std::expected<Term, std::string> repeat(
        Identity identity,
        repeat::Bounds bounds,
        Term body);
    static Term scope(
        Identity identity,
        scope::Policy policy,
        Term body);
    static Term bind(Identity identity, Identity binding, Term body);
    static Term stage(
        Identity identity,
        Identity stage,
        Term body);
    static Term candidate(
        Identity identity,
        Identity candidate,
        Term body);
    static Term produce(
        Identity identity,
        Identity production);

    [[nodiscard]] const Identity& identity() const noexcept;

private:
    std::shared_ptr<const detail::Node> node_;

    explicit Term(std::shared_ptr<const detail::Node> node);

    friend class detail::Runner;
};

struct Branch {
    Identity identity;
    choice::Cost cost;
    Term body;
};

} // namespace mq::kernel::grammar
