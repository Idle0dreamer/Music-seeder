#include "Internal.hpp"

#include <charconv>
#include <fstream>
#include <optional>
#include <sstream>

namespace mq::kernel::validation {
namespace {

std::string trim(std::string_view value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos) return {};
    const auto last = value.find_last_not_of(" \t\r\n");
    return std::string(value.substr(first, last - first + 1));
}

std::expected<std::int64_t, std::string> integer(std::string_view value) {
    std::int64_t result{};
    const auto parsed = std::from_chars(
        value.data(), value.data() + value.size(), result);
    if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size()) {
        return std::unexpected("invalid corpus integer: " + std::string(value));
    }
    return result;
}

std::expected<Rational, std::string> rational(std::string_view value) {
    const auto slash = value.find('/');
    if (slash == std::string_view::npos) {
        const auto parsed = integer(value);
        if (!parsed) return std::unexpected(parsed.error());
        return Rational(*parsed);
    }
    const auto numerator = integer(value.substr(0, slash));
    const auto denominator = integer(value.substr(slash + 1));
    if (!numerator) return std::unexpected(numerator.error());
    if (!denominator) return std::unexpected(denominator.error());
    if (*denominator == 0) {
        return std::unexpected("corpus rational denominator is zero");
    }
    return Rational(*numerator, *denominator);
}

std::vector<std::string_view> split(
    std::string_view value,
    char separator) {
    std::vector<std::string_view> result;
    std::size_t start{};
    while (start <= value.size()) {
        const auto end = value.find(separator, start);
        result.push_back(value.substr(
            start,
            end == std::string_view::npos ? end : end - start));
        if (end == std::string_view::npos) break;
        start = end + 1;
    }
    return result;
}

std::expected<std::vector<Observation>, std::string> observations(
    std::string_view value) {
    std::vector<Observation> result;
    for (const auto token : split(value, ';')) {
        const auto fields = split(trim(token), ':');
        if (fields.size() != 3) {
            return std::unexpected(
                "corpus observation requires duration:release:ornament");
        }
        const auto duration = rational(trim(fields[0]));
        const auto release = rational(trim(fields[1]));
        const auto ornament = rational(trim(fields[2]));
        if (!duration) return std::unexpected(duration.error());
        if (!release) return std::unexpected(release.error());
        if (!ornament) return std::unexpected(ornament.error());
        result.push_back({*duration, *release, *ornament});
    }
    return result;
}

std::expected<std::vector<Rational>, std::string> pauses(
    std::string_view value) {
    std::vector<Rational> result;
    if (trim(value).empty()) return result;
    for (const auto token : split(value, ';')) {
        const auto parsed = rational(trim(token));
        if (!parsed) return std::unexpected(parsed.error());
        result.push_back(*parsed);
    }
    return result;
}

} // namespace

namespace detail {

std::expected<Performance, std::string> finish(
    const Fields& fields,
    std::size_t record_number) {
    if (!fields.identity || !fields.profile || !fields.performer ||
        !fields.recording || !fields.split || !fields.observations ||
        !fields.pauses || !fields.evidence || !fields.confidence_seen ||
        !fields.disagreement_seen) {
        return std::unexpected(
            "corpus record " + std::to_string(record_number) +
            " is missing a required field");
    }
    Performance result{
        *fields.identity,
        *fields.profile,
        *fields.performer,
        *fields.recording,
        *fields.split,
        *fields.observations,
        *fields.pauses,
        *fields.evidence,
    };
    if (!result.well_formed()) {
        return std::unexpected(
            "corpus record " + std::to_string(record_number) +
            " is not well formed");
    }
    return result;
}

} // namespace detail

std::expected<Casebook, std::string> parse_casebook(std::string_view text) {
    std::string provenance;
    detail::Fields fields;
    bool active{};
    std::size_t record_number{};
    std::vector<Performance> performances;

    const auto finish_record = [&]() -> std::expected<void, std::string> {
        if (!active) return {};
        const auto result = detail::finish(fields, record_number);
        if (!result) return std::unexpected(result.error());
        performances.push_back(*result);
        fields = {};
        active = false;
        return {};
    };

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
        if (equals == std::string_view::npos) {
            return std::unexpected(
                "corpus line " + std::to_string(line_number) +
                " has no '='");
        }
        const auto key = trim(content.substr(0, equals));
        const auto value = trim(content.substr(equals + 1));
        if (key == "provenance") {
            if (!provenance.empty()) {
                return std::unexpected("corpus repeats provenance");
            }
            provenance = value;
            continue;
        }
        if (key == "record") {
            const auto previous = finish_record();
            if (!previous) return std::unexpected(previous.error());
            active = true;
            ++record_number;
            fields.identity = value;
        } else if (!active) {
            return std::unexpected(
                "corpus line " + std::to_string(line_number) +
                " appears before a record");
        } else if (key == "profile") {
            if (fields.profile) return std::unexpected("corpus repeats profile");
            fields.profile = value;
        } else if (key == "performer") {
            if (fields.performer) return std::unexpected("corpus repeats performer");
            fields.performer = value;
        } else if (key == "recording") {
            if (fields.recording) return std::unexpected("corpus repeats recording");
            fields.recording = value;
        } else if (key == "split") {
            if (fields.split) return std::unexpected("corpus repeats split");
            fields.split = value;
        } else if (key == "observations") {
            if (fields.observations) return std::unexpected("corpus repeats observations");
            const auto parsed = observations(value);
            if (!parsed) return std::unexpected(parsed.error());
            fields.observations = *parsed;
        } else if (key == "pauses") {
            if (fields.pauses) return std::unexpected("corpus repeats pauses");
            const auto parsed = pauses(value);
            if (!parsed) return std::unexpected(parsed.error());
            fields.pauses = *parsed;
        } else if (key == "source" || key == "location" || key == "rights" ||
                   key == "scope" || key == "annotator" ||
                   key == "confidence" || key == "disagreement") {
            if (!fields.evidence) fields.evidence = Evidence{};
            auto& evidence = *fields.evidence;
            if (key == "source") {
                if (!evidence.source.empty()) return std::unexpected("corpus repeats source");
                evidence.source = value;
            } else if (key == "location") {
                if (!evidence.location.empty()) return std::unexpected("corpus repeats location");
                evidence.location = value;
            } else if (key == "rights") {
                if (!evidence.rights.empty()) return std::unexpected("corpus repeats rights");
                evidence.rights = value;
            } else if (key == "scope") {
                if (!evidence.scope.empty()) return std::unexpected("corpus repeats scope");
                evidence.scope = value;
            } else if (key == "annotator") {
                if (!evidence.annotator.empty()) return std::unexpected("corpus repeats annotator");
                evidence.annotator = value;
            } else if (key == "confidence") {
                if (fields.confidence_seen) return std::unexpected("corpus repeats confidence");
                const auto parsed = rational(value);
                if (!parsed) return std::unexpected(parsed.error());
                evidence.confidence = *parsed;
                fields.confidence_seen = true;
            } else {
                if (fields.disagreement_seen) return std::unexpected("corpus repeats disagreement");
                const auto parsed = rational(value);
                if (!parsed) return std::unexpected(parsed.error());
                evidence.disagreement = *parsed;
                fields.disagreement_seen = true;
            }
        } else {
            return std::unexpected(
                "unknown corpus key at line " + std::to_string(line_number) +
                ": " + key);
        }
    }
    const auto last = finish_record();
    if (!last) return std::unexpected(last.error());
    Casebook result{std::move(provenance), std::move(performances)};
    if (!result.well_formed()) {
        return std::unexpected("corpus casebook is not well formed");
    }
    return result;
}

std::expected<Casebook, std::string> load_casebook(
    const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) {
        return std::unexpected("cannot open casebook: " + path.string());
    }
    std::ostringstream text;
    text << input.rdbuf();
    return parse_casebook(text.str());
}

} // namespace mq::kernel::validation
