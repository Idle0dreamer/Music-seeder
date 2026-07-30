#pragma once

#include "mq/kernel/pitch/field/Context.hpp"
#include "mq/kernel/pitch/field/project/Plan.hpp"
#include "mq/kernel/state/Snapshot.hpp"

#include <expected>
#include <optional>
#include <string>

namespace mq::kernel::pitch::field::project {

struct Limits {
    std::size_t sources{64};
};

struct Error {
    enum class Code {
        Input,
        Count,
        Missing,
    };

    Code code;
    std::string message;
    std::optional<Identity> source;
};

[[nodiscard]] std::expected<Context, Error> run(
    const Plan& plan,
    const state::Snapshot& state,
    Limits limits = {});

} // namespace mq::kernel::pitch::field::project
