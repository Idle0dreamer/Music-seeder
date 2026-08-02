#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

#include <string>

namespace mq::kernel::performance {

struct Pause {
    Rational onset;
    Rational duration;
    Identity function;
    std::string provenance;

    [[nodiscard]] bool well_formed() const {
        return onset >= Rational(0) && duration > Rational(0) &&
               !function.domain.empty() && !function.name.empty() &&
               !function.revision.empty() && !provenance.empty();
    }

    bool operator==(const Pause&) const = default;
};

} // namespace mq::kernel::performance
