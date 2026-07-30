#pragma once

#include "mq/kernel/Identity.hpp"

namespace mq::kernel::pitch::field::project::path {

struct Read {
    Identity key;
    Identity path;
    Identity complete;
    Identity incomplete;
};

} // namespace mq::kernel::pitch::field::project::path
