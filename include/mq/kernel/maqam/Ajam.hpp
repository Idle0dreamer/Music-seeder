#pragma once

#include "mq/kernel/maqam/Package.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_ajam();

} // namespace mq::kernel::maqam
