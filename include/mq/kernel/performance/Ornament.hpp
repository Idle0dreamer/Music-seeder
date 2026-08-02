#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/pitch/Expression.hpp"

namespace mq::kernel::performance {

// Fractions of the host event occupied by the ornament's attack, active
// motion, and release. These are performance-profile data, not maqam facts.
struct OrnamentTiming {
    Rational onset;
    Rational duration;
    Rational release;

    [[nodiscard]] bool well_formed() const {
        return onset >= Rational(0) && duration > Rational(0) &&
               release >= Rational(0) &&
               onset + duration + release <= Rational(1);
    }

    bool operator==(const OrnamentTiming&) const = default;
};

enum class OrnamentKind {
    Approach,
    Oscillation,
};

struct Ornament {
    Identity family;
    OrnamentKind kind{OrnamentKind::Approach};
    pitch::Expression extent{};
    Rational cycles{1};
    OrnamentTiming timing;
    std::string provenance;

    [[nodiscard]] bool well_formed() const {
        bool known = false;
        switch (kind) {
        case OrnamentKind::Approach:
        case OrnamentKind::Oscillation:
            known = true;
            break;
        }
        return known && cycles > Rational(0) && timing.well_formed() &&
               !family.domain.empty() && !family.name.empty() &&
               !family.revision.empty() && !provenance.empty();
    }

    bool operator==(const Ornament&) const = default;
};

} // namespace mq::kernel::performance
