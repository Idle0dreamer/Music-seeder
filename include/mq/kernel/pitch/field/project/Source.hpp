#pragma once

#include "mq/kernel/pitch/field/project/center/Read.hpp"
#include "mq/kernel/pitch/field/project/jins/Read.hpp"
#include "mq/kernel/pitch/field/project/path/Read.hpp"
#include "mq/kernel/pitch/field/project/tonicization/Read.hpp"

#include <variant>

namespace mq::kernel::pitch::field::project {

using Source = std::variant<
    center::Read,
    jins::Read,
    tonicization::Read,
    path::Read>;

} // namespace mq::kernel::pitch::field::project
