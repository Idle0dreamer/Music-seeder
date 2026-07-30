#pragma once

#include "mq/kernel/pitch/Feasibility.hpp"
#include "mq/kernel/pitch/field/assemble/Run.hpp"
#include "mq/kernel/pitch/tendency/sequence/Solve.hpp"

namespace mq::kernel::pitch::field::solve {

struct Limits {
    assemble::Limits assembly{};
    tendency::sequence::Limits tendency{};
    feasibility::Limits hard{};
};

struct Error {
    enum class Code {
        Assembly,
        Hard,
        Tendency,
        Internal,
    };

    Code code;
    std::string message;
    std::optional<assemble::Error> assembly;
    std::optional<feasibility::Error> hard;
    std::optional<tendency::sequence::Error> tendency;
};

struct Result {
    Solution solution;
    std::vector<tendency::sequence::Step> steps;
    std::vector<Identity> rules;
};

[[nodiscard]] std::expected<Result, Error> run(
    const Schema& schema,
    const Context& context,
    Limits limits = {});

} // namespace mq::kernel::pitch::field::solve
