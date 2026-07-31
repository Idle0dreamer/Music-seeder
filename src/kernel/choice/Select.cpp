#include "mq/kernel/choice/Select.hpp"

#include <algorithm>
#include <limits>
#include <tuple>

namespace mq::kernel::choice {
namespace {

std::uint64_t mix(std::uint64_t value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31U);
}

std::uint64_t text(std::uint64_t seed, const std::string& value) {
    auto result = mix(seed ^ static_cast<std::uint64_t>(value.size()));
    for (const auto character : value) {
        result = mix(result ^ static_cast<unsigned char>(character));
    }
    return result;
}

} // namespace

std::expected<Cost, std::string> sum(
    const Cost& left,
    const Cost& right) {
    Cost result;
    for (std::size_t index = 0; index < result.tiers.size(); ++index) {
        if (__builtin_add_overflow(
                left.tiers[index],
                right.tiers[index],
                &result.tiers[index])) {
            return std::unexpected("cost tier overflow");
        }
    }
    return result;
}

std::uint64_t key(
    std::uint64_t seed,
    std::span<const Draw> draws) {
    auto value = mix(seed);
    for (const auto& draw : draws) {
        value = text(value, draw.expression.str());
        for (const auto& item : draw.scope) {
            value = text(value, item.str());
        }
        value = text(value, draw.alternative.str());
    }
    return value;
}

std::expected<Identity, std::string> select(
    std::uint64_t seed,
    const Identity& expression,
    std::span<const Identity> scope,
    std::span<const Candidate> candidates,
    const Cost& band) {
    const Candidate* minimum = nullptr;

    for (const auto& candidate : candidates) {
        if (!candidate.eligible) {
            continue;
        }
        if (minimum == nullptr || candidate.cost < minimum->cost) {
            minimum = &candidate;
        }
    }

    if (minimum == nullptr) {
        return std::unexpected("choice has no eligible alternatives");
    }

    const auto limit = sum(minimum->cost, band);
    if (!limit) {
        return std::unexpected(limit.error());
    }

    const Candidate* best = nullptr;
    auto bestHash = std::numeric_limits<std::uint64_t>::max();

    for (const auto& candidate : candidates) {
        if (!candidate.eligible) {
            continue;
        }
        if (candidate.cost <= *limit) {
            const std::array draws{
                Draw{expression, {scope.begin(), scope.end()}, candidate.identity},
            };
            const auto candidateHash = key(seed, draws);
            if (best == nullptr || candidateHash < bestHash) {
                best = &candidate;
                bestHash = candidateHash;
            }
        }
    }

    return best->identity;
}

} // namespace mq::kernel::choice
