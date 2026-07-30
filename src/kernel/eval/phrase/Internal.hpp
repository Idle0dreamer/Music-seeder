#pragma once

#include "mq/kernel/eval/Evaluator.hpp"

namespace mq::kernel::eval::phrase {

[[nodiscard]] bool complete(const Identity& identity);

[[nodiscard]] Violation reject(
    std::size_t index,
    const char* operation,
    std::string rule,
    std::string message);

} // namespace mq::kernel::eval::phrase
