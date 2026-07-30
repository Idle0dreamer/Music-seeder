#include "Internal.hpp"

namespace mq::kernel::pitch::tendency::detail {
namespace {

Identity made(
    std::string domain,
    const Identity& source) {
    return {
        std::move(domain),
        source.str(),
        "1",
    };
}

Rational coefficient(
    std::span<const Term> terms,
    const Identity& variable) {
    Rational result;
    for (const auto& term : terms) {
        if (term.variable == variable) {
            result += term.coefficient;
        }
    }
    return result;
}

std::vector<Term> terms(const Form& form) {
    std::vector<Term> result;
    result.reserve(form.size());
    for (const auto& [variable, coefficient] : form) {
        if (coefficient != Rational(0)) {
            result.push_back({variable, coefficient});
        }
    }
    return result;
}

} // namespace

Problem build(
    const Model& model,
    std::span<const std::size_t> active) {
    Problem result{
        model.variables,
        model.equations,
        model.inequalities,
    };

    std::vector<Identity> equal;
    equal.reserve(model.equations.size());
    for (const auto& equation : model.equations) {
        equal.push_back(made(
            "mq.kernel.kkt.equal",
            equation.identity));
        result.variables.push_back(equal.back());
    }

    std::vector<Identity> dual;
    dual.reserve(active.size());
    for (const auto index : active) {
        const auto& inequality = model.inequalities.at(index);
        dual.push_back(made(
            "mq.kernel.kkt.bound",
            inequality.identity));
        result.variables.push_back(dual.back());
        result.equations.push_back({
            made("mq.kernel.kkt.active", inequality.identity),
            inequality.terms,
            inequality.right,
            inequality.provenance,
        });
        result.inequalities.push_back({
            made("mq.kernel.kkt.dual", inequality.identity),
            {{dual.back(), Rational(-1)}},
            {},
            "nonnegative KKT multiplier",
        });
    }

    for (const auto& variable : model.variables) {
        Form left;
        Expression right;
        for (std::size_t index = 0;
             index < model.targets.size();
             ++index) {
            const auto found = model.forms[index].find(variable);
            if (found == model.forms[index].end()) {
                continue;
            }
            const auto scale =
                Rational(2) *
                model.targets[index].weight *
                found->second;
            right += model.targets[index].target * scale;
            for (const auto& [other, value] : model.forms[index]) {
                left[other] += scale * value;
            }
        }
        for (std::size_t index = 0;
             index < model.equations.size();
             ++index) {
            const auto value =
                coefficient(model.equations[index].terms, variable);
            if (value != Rational(0)) {
                left[equal[index]] += value;
            }
        }
        for (std::size_t item = 0; item < active.size(); ++item) {
            const auto value = coefficient(
                model.inequalities[active[item]].terms,
                variable);
            if (value != Rational(0)) {
                left[dual[item]] += value;
            }
        }
        result.equations.push_back({
            made("mq.kernel.kkt.stationary", variable),
            terms(left),
            std::move(right),
            "KKT stationarity for " + variable.str(),
        });
    }
    return result;
}

} // namespace mq::kernel::pitch::tendency::detail
