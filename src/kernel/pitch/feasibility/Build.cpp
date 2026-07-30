#include "Internal.hpp"

#include <set>

namespace mq::kernel::pitch::feasibility::detail {
namespace {

std::expected<Row, Error> make(
    Identity identity,
    std::span<const Term> terms,
    Expression right,
    std::string provenance,
    const std::set<Identity>& variables,
    Rational scale) {
    Row row;
    row.right = std::move(right) * scale;
    row.constraints.push_back(std::move(identity));
    if (!provenance.empty()) {
        row.provenance.push_back(std::move(provenance));
    }
    for (const auto& term : terms) {
        if (!variables.contains(term.variable)) {
            return std::unexpected(Error{
                Error::Code::Input,
                "unknown pitch variable " + term.variable.str(),
                std::nullopt,
            });
        }
        row.left[term.variable] += term.coefficient * scale;
    }
    std::erase_if(
        row.left,
        [](const auto& item) { return item.second == Rational(0); });
    return row;
}

std::expected<void, Error> append(
    Rows& rows,
    std::expected<Row, Error> row,
    std::size_t limit) {
    if (!row) {
        return std::unexpected(row.error());
    }
    if (rows.size() >= limit) {
        return std::unexpected(Error{
            Error::Code::Rows,
            "pitch feasibility row budget exceeded while building constraints",
            std::nullopt,
        });
    }
    rows.push_back(std::move(*row));
    return {};
}

} // namespace

std::expected<Rows, Error> build(
    std::span<const Identity> variables,
    std::span<const Equation> equations,
    std::span<const Inequality> inequalities,
    std::size_t limit) {
    const std::set<Identity> known(variables.begin(), variables.end());
    if (known.size() != variables.size()) {
        return std::unexpected(Error{
            Error::Code::Input,
            "pitch feasibility variables contain a duplicate identity",
            std::nullopt,
        });
    }

    std::set<Identity> constraints;
    const auto registerConstraint =
        [&constraints](const Identity& identity) -> std::expected<void, Error> {
        if (identity.domain.empty() ||
            identity.name.empty() ||
            identity.revision.empty()) {
            return std::unexpected(Error{
                Error::Code::Input,
                "pitch constraint identity is incomplete",
                std::nullopt,
            });
        }
        if (!constraints.insert(identity).second) {
            return std::unexpected(Error{
                Error::Code::Input,
                "duplicate pitch constraint " + identity.str(),
                std::nullopt,
            });
        }
        return {};
    };
    for (const auto& equation : equations) {
        const auto added = registerConstraint(equation.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
    }
    for (const auto& inequality : inequalities) {
        const auto added = registerConstraint(inequality.identity);
        if (!added) {
            return std::unexpected(added.error());
        }
    }

    Rows rows;
    for (const auto& equation : equations) {
        auto upper = append(
            rows,
            make(
                equation.identity,
                equation.terms,
                equation.right,
                equation.provenance,
                known,
                Rational(1)),
            limit);
        if (!upper) {
            return std::unexpected(upper.error());
        }
        auto lower = append(
            rows,
            make(
                equation.identity,
                equation.terms,
                equation.right,
                equation.provenance,
                known,
                Rational(-1)),
            limit);
        if (!lower) {
            return std::unexpected(lower.error());
        }
    }
    for (const auto& inequality : inequalities) {
        auto result = append(
            rows,
            make(
                inequality.identity,
                inequality.terms,
                inequality.right,
                inequality.provenance,
                known,
                Rational(1)),
            limit);
        if (!result) {
            return std::unexpected(result.error());
        }
    }
    return rows;
}

} // namespace mq::kernel::pitch::feasibility::detail
