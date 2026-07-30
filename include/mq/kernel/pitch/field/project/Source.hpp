#pragma once

#include "mq/kernel/pitch/field/project/center/Read.hpp"
#include "mq/kernel/pitch/field/project/jins/Read.hpp"
#include "mq/kernel/pitch/field/project/motion/Read.hpp"
#include "mq/kernel/pitch/field/project/path/Read.hpp"
#include "mq/kernel/pitch/field/project/phrase/boundary/Read.hpp"
#include "mq/kernel/pitch/field/project/phrase/cadence/Read.hpp"
#include "mq/kernel/pitch/field/project/phrase/function/Read.hpp"
#include "mq/kernel/pitch/field/project/role/Read.hpp"
#include "mq/kernel/pitch/field/project/tonicization/Read.hpp"

#include <variant>

namespace mq::kernel::pitch::field::project {

using Source = std::variant<
    center::Read,
    jins::Read,
    role::Read,
    motion::Read,
    tonicization::Read,
    path::Read,
    phrase::function::Read,
    phrase::cadence::Read,
    phrase::boundary::Read>;

} // namespace mq::kernel::pitch::field::project
