#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/pitch/Expression.hpp"

namespace mq::kernel::performance {

enum class OrnamentKind {
    Approach,
    Oscillation,
};

struct Ornament {
    Identity family;
    OrnamentKind kind{OrnamentKind::Approach};
    pitch::Expression extent{};
    Rational cycles{1};
    std::string provenance;

    [[nodiscard]] bool well_formed() const {
        bool known = false;
        switch (kind) {
        case OrnamentKind::Approach:
        case OrnamentKind::Oscillation:
            known = true;
            break;
        }
        return known && cycles > Rational(0) &&
               !family.domain.empty() && !family.name.empty() &&
               !family.revision.empty() && !provenance.empty();
    }

    bool operator==(const Ornament&) const = default;
};

} // namespace mq::kernel::performance
