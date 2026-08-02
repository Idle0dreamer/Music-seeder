#pragma once

#include "mq/kernel/maqam/Package.hpp"

namespace mq::kernel::maqam {

std::expected<Scaffold, std::string> make_hijaz();

} // namespace mq::kernel::maqam
