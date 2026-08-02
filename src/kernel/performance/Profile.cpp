#include "mq/kernel/performance/Profile.hpp"

#include <array>
#include <charconv>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

namespace mq::kernel::performance {
namespace {

struct Fields {
    std::optional<Rational> duration;
    std::optional<Rational> intensity;
    std::optional<Articulation> articulation;
};

std::string trim(std::string_view value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos) {
        return {};
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    return std::string(value.substr(first, last - first + 1));
}

std::expected<std::int64_t, std::string> integer(std::string_view value) {
    std::int64_t result{};
    const auto* first = value.data();
    const auto* last = first + value.size();
    const auto parsed = std::from_chars(first, last, result);
    if (parsed.ec != std::errc{} || parsed.ptr != last) {
        return std::unexpected("invalid integer: " + std::string(value));
    }
    return result;
}

std::expected<Rational, std::string> rational(std::string_view value) {
    const auto slash = value.find('/');
    if (slash == std::string_view::npos) {
        const auto numerator = integer(value);
        if (!numerator) {
            return std::unexpected(numerator.error());
        }
        return Rational(*numerator);
    }
    const auto numerator = integer(value.substr(0, slash));
    const auto denominator = integer(value.substr(slash + 1));
    if (!numerator) {
        return std::unexpected(numerator.error());
    }
    if (!denominator) {
        return std::unexpected(denominator.error());
    }
    if (*denominator == 0) {
        return std::unexpected("rational denominator is zero");
    }
    return Rational(*numerator, *denominator);
}

std::expected<Articulation, std::string> articulation(
    std::string_view value) {
    if (value == "neutral") return Articulation::Neutral;
    if (value == "connected") return Articulation::Connected;
    if (value == "detached") return Articulation::Detached;
    return std::unexpected("unknown articulation: " + std::string(value));
}

std::expected<std::size_t, std::string> direction_index(
    std::string_view value) {
    if (value == "start") return 0;
    if (value == "same") return 1;
    if (value == "rise") return 2;
    if (value == "fall") return 3;
    return std::unexpected("unknown timing direction: " + std::string(value));
}

std::expected<TimingIntent, std::string> finish(
    const Fields& fields,
    std::string_view direction) {
    if (!fields.duration || !fields.intensity || !fields.articulation) {
        return std::unexpected(
            "timing profile is missing fields for " + std::string(direction));
    }
    TimingIntent intent{
        *fields.duration,
        *fields.intensity,
        *fields.articulation,
    };
    if (!intent.well_formed()) {
        return std::unexpected(
            "timing profile has an invalid intent for " +
            std::string(direction));
    }
    return intent;
}

} // namespace

std::expected<Timing, std::string> parse_timing_profile(
    std::string_view text) {
    std::array<Fields, 4> fields;
    std::string provenance;
    std::istringstream input{std::string(text)};
    std::string line;
    std::size_t line_number{};
    while (std::getline(input, line)) {
        ++line_number;
        const auto comment = line.find('#');
        const auto content = trim(
            comment == std::string::npos
                ? std::string_view(line)
                : std::string_view(line).substr(0, comment));
        if (content.empty()) continue;
        const auto equals = content.find('=');
        if (equals == std::string::npos) {
            return std::unexpected(
                "timing profile line " + std::to_string(line_number) +
                " has no '='");
        }
        const auto key = trim(std::string_view(content).substr(0, equals));
        const auto value = trim(std::string_view(content).substr(equals + 1));
        if (key == "provenance") {
            if (!provenance.empty()) {
                return std::unexpected("timing profile repeats provenance");
            }
            provenance = value;
            continue;
        }
        const auto first = key.find('.');
        const auto second = key.find('.', first == std::string::npos ? first : first + 1);
        if (first == std::string::npos || second == std::string::npos ||
            second + 1 >= key.size()) {
            return std::unexpected("unknown timing profile key: " + key);
        }
        const auto direction = direction_index(key.substr(0, first));
        if (!direction) return std::unexpected(direction.error());
        const auto field = key.substr(second + 1);
        if (key.substr(first + 1, second - first - 1) != "intent") {
            return std::unexpected("unknown timing profile key: " + key);
        }
        auto& destination = fields[*direction];
        if (field == "duration") {
            if (destination.duration) return std::unexpected("timing profile repeats " + key);
            const auto parsed = rational(value);
            if (!parsed) return std::unexpected(parsed.error());
            destination.duration = *parsed;
        } else if (field == "intensity") {
            if (destination.intensity) return std::unexpected("timing profile repeats " + key);
            const auto parsed = rational(value);
            if (!parsed) return std::unexpected(parsed.error());
            destination.intensity = *parsed;
        } else if (field == "articulation") {
            if (destination.articulation) return std::unexpected("timing profile repeats " + key);
            const auto parsed = articulation(value);
            if (!parsed) return std::unexpected(parsed.error());
            destination.articulation = *parsed;
        } else {
            return std::unexpected("unknown timing profile key: " + key);
        }
    }
    if (provenance.empty()) {
        return std::unexpected("timing profile has no provenance");
    }
    const auto start = finish(fields[0], "start");
    const auto same = finish(fields[1], "same");
    const auto rise = finish(fields[2], "rise");
    const auto fall = finish(fields[3], "fall");
    if (!start) return std::unexpected(start.error());
    if (!same) return std::unexpected(same.error());
    if (!rise) return std::unexpected(rise.error());
    if (!fall) return std::unexpected(fall.error());
    return Timing{*start, *same, *rise, *fall, std::move(provenance)};
}

std::expected<Timing, std::string> load_timing_profile(
    const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) {
        return std::unexpected(
            "cannot open timing profile: " + path.string());
    }
    std::ostringstream text;
    text << input.rdbuf();
    return parse_timing_profile(text.str());
}

} // namespace mq::kernel::performance
