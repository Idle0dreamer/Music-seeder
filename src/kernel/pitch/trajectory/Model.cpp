#include "mq/kernel/pitch/trajectory/Model.hpp"

namespace mq::kernel::pitch::trajectory {

double Formula::evaluate(double t) const noexcept {
    double sum = 0.0;
    for (const auto& term : terms) {
        sum += term.weight * term.basis.evaluate(t);
    }
    return sum;
}

Formula Model::resolve(const IntoneContext& context) const {
    Formula result = base;

    if (context.ascending) {
        for (const auto& term : ascent.terms) {
            result.terms.push_back(term);
        }
    } else if (context.descending) {
        for (const auto& term : descent.terms) {
            result.terms.push_back(term);
        }
    }

    if (context.cadence) {
        for (const auto& term : cadence_approach.terms) {
            result.terms.push_back(term);
        }
    }

    if (context.emphasis) {
        for (const auto& term : emphasis_attraction.terms) {
            result.terms.push_back(term);
        }
    }

    return result;
}

} // namespace mq::kernel::pitch::trajectory
