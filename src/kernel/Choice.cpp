#include "mq/kernel/Choice.hpp"

#include <algorithm>
#include <limits>
#include <tuple>

namespace mq::kernel {
namespace {

std::uint64_t mix(std::uint64_t value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31U);
}

std::uint64_t hashText(std::uint64_t seed, const std::string& text) {
    auto result = seed;
    for (const auto character : text) {
        result = mix(result ^ static_cast<unsigned char>(character));
    }
    return result;
}

std::uint64_t choiceHash(
    std::uint64_t seed,
    const Identity& expression,
    const std::string& scopePath,
    const Identity& alternative) {
    auto value = hashText(seed, expression.str());
    value = hashText(value, scopePath);
    return hashText(value, alternative.str());
}

} // namespace

std::expected<Identity, std::string> select(
    std::uint64_t seed,
    const Identity& expression,
    const std::string& scopePath,
    std::span<const Candidate> candidates) {
    const Candidate* best = nullptr;
    auto bestHash = std::numeric_limits<std::uint64_t>::max();

    for (const auto& candidate : candidates) {
        if (!candidate.eligible) {
            continue;
        }
        if (best == nullptr || candidate.cost < best->cost) {
            best = &candidate;
            bestHash = choiceHash(
                seed,
                expression,
                scopePath,
                candidate.identity);
            continue;
        }
        if (candidate.cost == best->cost) {
            const auto candidateHash = choiceHash(
                seed,
                expression,
                scopePath,
                candidate.identity);
            if (candidateHash < bestHash) {
                best = &candidate;
                bestHash = candidateHash;
            }
        }
    }

    if (best == nullptr) {
        return std::unexpected("choice has no eligible alternatives");
    }
    return best->identity;
}

} // namespace mq::kernel

