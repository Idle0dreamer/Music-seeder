#include "Internal.hpp"

#include <charconv>
#include <cstdint>
#include <map>
#include <set>
#include <string_view>

namespace mq::kernel::maqam::family::detail {
namespace {

std::expected<Identity, std::string> reference(
    const Key& key,
    std::string_view kind,
    const std::string& name) {
    if (name.empty() || name == "-") {
        return std::unexpected(
            "empty " + std::string(kind) + " reference in " + key.package);
    }
    if (kind == "obligation") {
        return id(
            key,
            name.starts_with("obligation.") ? name : "obligation." + name);
    }
    const auto token = name.find('.') == std::string::npos
                           ? std::string(kind) + "." + name
                           : name;
    const auto found = key.authorities.find(token);
    if (found == key.authorities.end()) {
        return std::unexpected(
            "undeclared " + std::string(kind) + " reference " + token +
            " in " + key.package);
    }
    return found->second;
}

std::expected<std::size_t, std::string> count(
    const std::string& value,
    std::string_view label) {
    std::size_t result{};
    const auto parsed = std::from_chars(
        value.data(), value.data() + value.size(), result);
    if (parsed.ec != std::errc{} ||
        parsed.ptr != value.data() + value.size()) {
        return std::unexpected(
            "invalid " + std::string(label) + ": " + value);
    }
    return result;
}

std::expected<Rational, std::string> rational(const std::string& value) {
    const auto separator = value.find('/');
    const auto numeratorText = value.substr(
        0,
        separator == std::string::npos ? value.size() : separator);
    const auto denominatorText = separator == std::string::npos
                                     ? std::string("1")
                                     : value.substr(separator + 1);
    std::int64_t numerator{};
    std::int64_t denominator{};
    const auto parsedNumerator = std::from_chars(
        numeratorText.data(), numeratorText.data() + numeratorText.size(),
        numerator);
    const auto parsedDenominator = std::from_chars(
        denominatorText.data(), denominatorText.data() + denominatorText.size(),
        denominator);
    if (parsedNumerator.ec != std::errc{} ||
        parsedNumerator.ptr != numeratorText.data() + numeratorText.size() ||
        parsedDenominator.ec != std::errc{} ||
        parsedDenominator.ptr != denominatorText.data() + denominatorText.size() ||
        denominator <= 0) {
        return std::unexpected("invalid sayr rational: " + value);
    }
    return Rational(numerator, denominator);
}

std::expected<phrase::Boundary, std::string> boundary(
    const std::string& value) {
    if (value == "open") return phrase::Boundary::Open;
    if (value == "closed") return phrase::Boundary::Closed;
    return std::unexpected("invalid sayr phrase boundary: " + value);
}

std::expected<evidence::Kind, std::string> evidenceKind(
    const std::string& value) {
    static const std::map<std::string, evidence::Kind> values{
        {"recurrence", evidence::Kind::Recurrence},
        {"dwell", evidence::Kind::Dwell},
        {"emphasis", evidence::Kind::Emphasis},
        {"cell", evidence::Kind::Cell},
        {"baggage", evidence::Kind::Baggage},
        {"cadence", evidence::Kind::Cadence},
        {"register", evidence::Kind::Register},
    };
    const auto found = values.find(value);
    if (found == values.end()) {
        return std::unexpected("invalid sayr evidence kind: " + value);
    }
    return found->second;
}

std::expected<sayr::Need, std::string> need(
    const Key& key,
    const std::string& obligation,
    const NeedSpec& specification) {
    const auto& args = specification.arguments;
    const auto identity = [&] {
        return id(
            key,
            "need." + obligation + "." + specification.kind + "." +
                (args.empty() ? std::string("none") : args.front()));
    };
    if (specification.kind == "jins" && args.size() == 1) {
        const auto value = reference(key, "jins", args[0]);
        if (!value) return std::unexpected(value.error());
        return sayr::Need{
            identity(),
            sayr::need::Jins{*value}};
    }
    if (specification.kind == "center" && args.size() == 1) {
        const auto value = reference(key, "center", args[0]);
        if (!value) return std::unexpected(value.error());
        return sayr::Need{
            identity(),
            sayr::need::Center{*value}};
    }
    if (specification.kind == "path" && args.size() == 1) {
        const auto value = reference(key, "path", args[0]);
        if (!value) return std::unexpected(value.error());
        return sayr::Need{
            identity(),
            sayr::need::Path{*value}};
    }
    if (specification.kind == "role" && (args.size() == 1 || args.size() == 2)) {
        const auto value = reference(key, "role", args[0]);
        const auto minimum = args.size() == 2
                                 ? count(args[1], "role minimum")
                                 : std::expected<std::size_t, std::string>{1};
        if (!value) return std::unexpected(value.error());
        if (!minimum) return std::unexpected(minimum.error());
        return sayr::Need{
            identity(),
            sayr::need::Role{*value, *minimum}};
    }
    if (specification.kind == "region" && (args.size() == 1 || args.size() == 2)) {
        const auto value = reference(key, "region", args[0]);
        const auto minimum = args.size() == 2
                                 ? count(args[1], "region minimum")
                                 : std::expected<std::size_t, std::string>{1};
        if (!value) return std::unexpected(value.error());
        if (!minimum) return std::unexpected(minimum.error());
        return sayr::Need{
            identity(),
            sayr::need::Region{*value, *minimum}};
    }
    if (specification.kind == "gesture" && (args.size() == 1 || args.size() == 2)) {
        const auto value = reference(key, "gesture", args[0]);
        const auto minimum = args.size() == 2
                                 ? count(args[1], "gesture minimum")
                                 : std::expected<std::size_t, std::string>{1};
        if (!value) return std::unexpected(value.error());
        if (!minimum) return std::unexpected(minimum.error());
        return sayr::Need{
            identity(),
            sayr::need::Gesture{*value, *minimum}};
    }
    if (specification.kind == "evidence" && args.size() == 2) {
        const auto kind = evidenceKind(args[0]);
        const auto minimum = rational(args[1]);
        if (!kind) return std::unexpected(kind.error());
        if (!minimum) return std::unexpected(minimum.error());
        return sayr::Need{
            identity(),
            sayr::need::Evidence{*kind, *minimum}};
    }
    if (specification.kind == "phrase" &&
        (args.size() == 3 || args.size() == 4)) {
        const auto function = reference(key, "phrase", args[0]);
        const auto phraseBoundary = boundary(args[1]);
        std::optional<Identity> cadence;
        if (args[2] != "-") {
            const auto value = reference(key, "cadence", args[2]);
            if (!value) return std::unexpected(value.error());
            cadence = *value;
        }
        const auto minimum = args.size() == 4
                                 ? count(args[3], "phrase minimum")
                                 : std::expected<std::size_t, std::string>{1};
        if (!function) return std::unexpected(function.error());
        if (!phraseBoundary) return std::unexpected(phraseBoundary.error());
        if (!minimum) return std::unexpected(minimum.error());
        return sayr::Need{
            identity(),
            sayr::need::Phrase{
                *function,
                *phraseBoundary,
                cadence,
                *minimum}};
    }
    return std::unexpected(
        "invalid sayr need declaration: " + specification.kind);
}

} // namespace

std::expected<sayr::Plan, std::string> sayr(const Key& key) {
    if (key.obligations.empty()) {
        return std::unexpected(
            "family package has no collection sayr obligations: " +
            key.package);
    }
    std::vector<sayr::Obligation> obligations;
    obligations.reserve(key.obligations.size());
    for (const auto& specification : key.obligations) {
        std::vector<sayr::Need> needs;
        needs.reserve(specification.needs.size());
        for (const auto& source : specification.needs) {
            const auto translated = need(key, specification.name, source);
            if (!translated) return std::unexpected(translated.error());
            needs.push_back(*translated);
        }
        if (needs.empty()) {
            return std::unexpected(
                "collection sayr obligation has no needs: " +
                specification.name);
        }
        std::set<Identity> after;
        for (const auto& predecessor : specification.after) {
            const auto value = reference(key, "obligation", predecessor);
            if (!value) return std::unexpected(value.error());
            after.insert(*value);
        }
        const auto identity = reference(
            key, "obligation", specification.name);
        if (!identity) return std::unexpected(identity.error());
        obligations.push_back({
            *identity,
            std::move(needs),
            std::move(after)});
    }
    std::vector<sayr::Route> routes;
    routes.reserve(key.routes.size());
    for (const auto& route : key.routes) {
        if (route.terminals.empty()) {
            return std::unexpected(
                "collection route has no sayr terminals: " +
                route.route.str());
        }
        std::set<Identity> terminals(
            route.terminals.begin(), route.terminals.end());
        routes.push_back({route.route, std::move(terminals)});
    }
    auto result = sayr::Plan::make(
        id(key, "sayr.canonical"),
        std::move(obligations),
        std::move(routes));
    if (!result) return std::unexpected(result.error());
    return result;
}

} // namespace mq::kernel::maqam::family::detail
