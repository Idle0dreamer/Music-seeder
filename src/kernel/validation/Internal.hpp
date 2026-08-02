#pragma once

#include "mq/kernel/validation/Casebook.hpp"

#include <optional>

namespace mq::kernel::validation::detail {

struct Fields {
    std::optional<std::string> identity;
    std::optional<std::string> profile;
    std::optional<std::string> performer;
    std::optional<std::string> recording;
    std::optional<std::string> split;
    std::optional<std::vector<Observation>> observations;
    std::optional<std::vector<Rational>> pauses;
    std::optional<Evidence> evidence;
    bool confidence_seen{};
    bool disagreement_seen{};
};

[[nodiscard]] std::expected<Performance, std::string> finish(
    const Fields& fields,
    std::size_t record_number);

} // namespace mq::kernel::validation::detail
