#pragma once

#include "mq/kernel/pitch/Equation.hpp"
#include "mq/kernel/pitch/Inequality.hpp"
#include "mq/kernel/pitch/field/Aim.hpp"

#include <variant>

namespace mq::kernel::pitch::field {

using Effect = std::variant<Equation, Inequality, Aim>;

} // namespace mq::kernel::pitch::field
