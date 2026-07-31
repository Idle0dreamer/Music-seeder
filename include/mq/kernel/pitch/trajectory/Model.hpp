#pragma once

#include "mq/kernel/pitch/trajectory/Basis.hpp"

#include <vector>

namespace mq::kernel::pitch::trajectory {

struct Term {
    double weight;
    Basis basis;
};

struct Formula {
    std::vector<Term> terms;

    [[nodiscard]] double evaluate(double t) const noexcept;
};

struct IntoneContext {
    // These could be Identity references in a real system,
    // but here we just represent the contextual properties.
    bool ascending;
    bool descending;
    bool cadence;
    bool emphasis;
    // ... other properties
};

// A Model evaluates a contextual formula
struct Model {
    Formula base;
    Formula ascent;
    Formula descent;
    Formula cadence_approach;
    Formula emphasis_attraction;

    [[nodiscard]] Formula resolve(const IntoneContext& context) const;
};

} // namespace mq::kernel::pitch::trajectory
