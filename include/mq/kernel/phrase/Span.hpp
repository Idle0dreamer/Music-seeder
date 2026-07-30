#pragma once

#include "mq/kernel/phrase/Boundary.hpp"
#include "mq/kernel/phrase/Frame.hpp"

namespace mq::kernel::phrase {

struct Span {
    Identity identity;
    Function function;
    Identity first;
    Identity last;
    Boundary boundary{Boundary::Open};
    std::vector<Cadence> cadences;

    bool operator==(const Span&) const = default;
};

} // namespace mq::kernel::phrase
