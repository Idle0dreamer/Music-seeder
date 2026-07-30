#include "Internal.hpp"

namespace mq::kernel::pitch::order::detail {

std::expected<Proof, Error> prove(Powers powers, Limits limits) {
    std::erase_if(
        powers,
        [](const auto& item) { return item.second == Rational(0); });
    if (powers.empty()) {
        return Proof{Relation::Equal, {}, 1, "1", "1"};
    }
    const auto common = scale(powers);
    if (!common) {
        return std::unexpected(common.error());
    }
    const auto positive = product(powers, *common, true, limits);
    if (!positive) {
        return std::unexpected(positive.error());
    }
    const auto negative = product(powers, *common, false, limits);
    if (!negative) {
        return std::unexpected(negative.error());
    }

    if (*positive == *negative) {
        return std::unexpected(Error{
            Error::Code::Prime,
            "non-empty prime basis produced an equal order certificate",
        });
    }
    const auto relation = *positive < *negative
                            ? Relation::Less
                            : Relation::Greater;
    return Proof{
        relation,
        std::move(powers),
        *common,
        positive->str(),
        negative->str(),
    };
}

} // namespace mq::kernel::pitch::order::detail

namespace mq::kernel::pitch::order {

std::expected<Proof, Error> compare(
    const Expression& left,
    const Expression& right,
    Limits limits) {
    return detail::prove(detail::normalize(left, right), limits);
}

std::expected<bool, Error> verify(
    const Proof& proof,
    Limits limits) {
    const auto rebuilt = detail::prove(proof.normalized, limits);
    if (!rebuilt) {
        return std::unexpected(rebuilt.error());
    }
    return *rebuilt == proof;
}

} // namespace mq::kernel::pitch::order
