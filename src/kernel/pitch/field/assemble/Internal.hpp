#pragma once

#include "mq/kernel/pitch/field/assemble/Run.hpp"

#include <map>
#include <set>
#include <span>

namespace mq::kernel::pitch::field::assemble::detail {

using Facts = std::map<Identity, Identity>;

[[nodiscard]] bool complete(const Identity& value);

[[nodiscard]] Error input(
    std::string message,
    std::optional<Identity> source = {});

[[nodiscard]] std::expected<void, Error> add(
    std::set<Identity>& values,
    const Identity& value);

[[nodiscard]] std::expected<void, Error> check(
    std::span<const Fact> source,
    Facts* result = nullptr);

[[nodiscard]] std::expected<void, Error> check(
    const Rule& rule,
    const std::set<Identity>& variables,
    const std::set<Identity>& tiers,
    std::set<Identity>& all,
    std::set<Identity>& rules);

[[nodiscard]] std::expected<void, Error> check(
    const Need& need,
    const std::set<Identity>& rules,
    std::set<Identity>& all);

[[nodiscard]] Identity identity(const Rule& rule);

[[nodiscard]] bool match(
    const Guard& guard,
    const Facts& context);

[[nodiscard]] std::expected<Facts, Error> validate(
    const Schema& schema,
    const Context& context,
    Limits limits);

} // namespace mq::kernel::pitch::field::assemble::detail
