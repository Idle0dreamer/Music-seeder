#include "Internal.hpp"

namespace mq::kernel::pitch::field::assemble::detail {

bool complete(const Identity& value) {
    return !value.domain.empty() &&
           !value.name.empty() &&
           !value.revision.empty();
}

Error input(
    std::string message,
    std::optional<Identity> source) {
    return {Error::Code::Input, std::move(message), std::move(source)};
}

std::expected<void, Error> add(
    std::set<Identity>& values,
    const Identity& value) {
    if (!complete(value)) {
        return std::unexpected(input("field contains an incomplete identity"));
    }
    if (value.domain.starts_with("mq.kernel.kkt") ||
        value.domain.starts_with("mq.kernel.sequence")) {
        return std::unexpected(input(
            "field uses a reserved solver identity domain",
            value));
    }
    if (!values.insert(value).second) {
        return std::unexpected(input(
            "field repeats identity " + value.str(),
            value));
    }
    return {};
}

std::expected<void, Error> check(
    std::span<const Fact> source,
    Facts* result) {
    std::set<Identity> keys;
    for (const auto& fact : source) {
        if (!complete(fact.key) || !complete(fact.value)) {
            return std::unexpected(input(
                "field fact contains an incomplete identity"));
        }
        if (!keys.insert(fact.key).second) {
            return std::unexpected(input(
                "field repeats context key " + fact.key.str(),
                fact.key));
        }
        if (result != nullptr) {
            result->emplace(fact.key, fact);
        }
    }
    return {};
}

} // namespace mq::kernel::pitch::field::assemble::detail
