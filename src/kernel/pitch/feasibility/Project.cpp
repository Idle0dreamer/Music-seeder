#include "Internal.hpp"

namespace mq::kernel::pitch::feasibility::detail {
namespace {

std::expected<std::size_t, Error> retain(
    std::size_t used,
    std::size_t rows,
    std::size_t limit,
    bool header) {
    const auto overhead = header ? 1U : 0U;
    if (used > limit ||
        overhead > limit - used ||
        rows > limit - used - overhead) {
        return std::unexpected(Error{
            Error::Code::Stages,
            "pitch feasibility retained-stage budget exceeded",
            std::nullopt,
        });
    }
    return used + overhead + rows;
}

} // namespace

std::expected<Projection, Error> project(
    Rows rows,
    std::span<const Identity> variables,
    Limits limits) {
    Projection result;
    result.stages.reserve(variables.size());
    std::size_t retained = 0;
    for (const auto& variable : variables) {
        const auto next =
            retain(retained, rows.size(), limits.retained, true);
        if (!next) {
            return std::unexpected(next.error());
        }
        retained = *next;
        result.stages.push_back(rows);
        auto projected =
            eliminate(std::move(rows), variable, limits.rows);
        if (!projected) {
            return std::unexpected(projected.error());
        }
        rows = std::move(*projected);
    }
    const auto next =
        retain(retained, rows.size(), limits.retained, false);
    if (!next) {
        return std::unexpected(next.error());
    }
    result.final = std::move(rows);
    return result;
}

} // namespace mq::kernel::pitch::feasibility::detail
