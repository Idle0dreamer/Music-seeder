#pragma once

#include "mq/kernel/maqam/Bayati.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_rast();

} // namespace mq::kernel::maqam
