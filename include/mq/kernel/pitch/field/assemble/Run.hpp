#pragma once

#include "mq/kernel/pitch/field/Context.hpp"
#include "mq/kernel/pitch/field/Model.hpp"
#include "mq/kernel/pitch/field/Schema.hpp"

#include <expected>
#include <optional>
#include <string>

namespace mq::kernel::pitch::field::assemble {

struct Limits {
    std::size_t facts{64};
    std::size_t rules{4096};
    std::size_t needs{512};
};

struct Error {
    enum class Code {
        Input,
        Count,
        Coverage,
    };

    Code code;
    std::string message;
    std::optional<Identity> source;
};

[[nodiscard]] std::expected<Model, Error> run(
    const Schema& schema,
    const Context& context,
    Limits limits = {});

} // namespace mq::kernel::pitch::field::assemble
