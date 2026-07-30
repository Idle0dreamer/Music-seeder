#include "Internal.hpp"

#include <algorithm>

namespace mq::kernel::pitch::feasibility::detail {
namespace {

void merge(
    std::vector<std::string>& target,
    const std::vector<std::string>& source) {
    for (const auto& item : source) {
        if (std::ranges::find(target, item) == target.end()) {
            target.push_back(item);
        }
    }
}

Row combine(
    const Row& upper,
    Rational upperScale,
    const Row& lower,
    Rational lowerScale) {
    Row result;
    for (const auto& [variable, coefficient] : upper.left) {
        result.left[variable] += coefficient * upperScale;
    }
    for (const auto& [variable, coefficient] : lower.left) {
        result.left[variable] += coefficient * lowerScale;
    }
    std::erase_if(
        result.left,
        [](const auto& item) { return item.second == Rational(0); });
    result.right =
        upper.right * upperScale + lower.right * lowerScale;
    result.provenance = upper.provenance;
    merge(result.provenance, lower.provenance);
    return result;
}

std::expected<std::size_t, Error> count(
    std::size_t neutral,
    std::size_t positive,
    std::size_t negative,
    std::size_t limit) {
    if (neutral > limit) {
        return std::unexpected(Error{
            Error::Code::Rows,
            "pitch feasibility row budget exceeded during elimination",
            std::nullopt,
        });
    }
    const auto remaining = limit - neutral;
    if (negative != 0 && positive > remaining / negative) {
        return std::unexpected(Error{
            Error::Code::Rows,
            "pitch feasibility row budget exceeded during elimination",
            std::nullopt,
        });
    }
    return neutral + positive * negative;
}

} // namespace

std::expected<Rows, Error> eliminate(
    Rows rows,
    const Identity& variable,
    std::size_t limit) {
    Rows neutral;
    Rows positive;
    Rows negative;
    for (auto& row : rows) {
        const auto found = row.left.find(variable);
        if (found == row.left.end()) {
            neutral.push_back(std::move(row));
        } else if (found->second > Rational(0)) {
            positive.push_back(std::move(row));
        } else {
            negative.push_back(std::move(row));
        }
    }

    const auto size =
        count(neutral.size(), positive.size(), negative.size(), limit);
    if (!size) {
        return std::unexpected(size.error());
    }
    neutral.reserve(*size);
    for (const auto& upper : positive) {
        const auto upperCoefficient = upper.left.at(variable);
        for (const auto& lower : negative) {
            const auto lowerCoefficient = lower.left.at(variable);
            neutral.push_back(combine(
                upper,
                -lowerCoefficient,
                lower,
                upperCoefficient));
        }
    }
    return neutral;
}

} // namespace mq::kernel::pitch::feasibility::detail
