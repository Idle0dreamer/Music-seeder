#include "mq/kernel/maqam/collection/Loader.hpp"

#include "mq/kernel/motion/Direction.hpp"

#include <charconv>
#include <cstdlib>
#include <fstream>
#include <sstream>

namespace mq::kernel::maqam::collection {
namespace {

std::string trim(std::string value) {
    const auto first = value.find_first_not_of(" \t\r");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = value.find_last_not_of(" \t\r");
    return value.substr(first, last - first + 1);
}

std::vector<std::string> split(
    const std::string& value,
    char delimiter) {
    std::vector<std::string> result;
    std::size_t start = 0;
    while (start <= value.size()) {
        const auto end = value.find(delimiter, start);
        result.push_back(trim(value.substr(
            start,
            end == std::string::npos ? std::string::npos : end - start)));
        if (end == std::string::npos) {
            break;
        }
        start = end + 1;
    }
    return result;
}

std::expected<std::int64_t, std::string> integer(
    const std::string& value,
    const std::string& field,
    std::size_t line) {
    std::int64_t result{};
    const auto* first = value.data();
    const auto* last = first + value.size();
    const auto parsed = std::from_chars(first, last, result);
    if (parsed.ec != std::errc{} || parsed.ptr != last) {
        return std::unexpected(
            "invalid integer for " + field + " at line " +
            std::to_string(line));
    }
    return result;
}

std::expected<pitch::Expression, std::string> ratio(
    const std::string& value,
    const std::string& field,
    std::size_t line) {
    const auto parts = split(value, '/');
    if (parts.size() != 2 || parts[0].empty() || parts[1].empty()) {
        return std::unexpected(
            "expected numerator/denominator for " + field +
            " at line " + std::to_string(line));
    }
    const auto numerator = integer(parts[0], field, line);
    if (!numerator) {
        return std::unexpected(numerator.error());
    }
    const auto denominator = integer(parts[1], field, line);
    if (!denominator) {
        return std::unexpected(denominator.error());
    }
    if (*denominator <= 0) {
        return std::unexpected(
            "ratio denominator must be positive for " + field +
            " at line " + std::to_string(line));
    }
    return pitch::Expression::ratio(*numerator, *denominator);
}

std::expected<bool, std::string> boolean(
    const std::string& value,
    const std::string& field,
    std::size_t line) {
    if (value == "true") {
        return true;
    }
    if (value == "false") {
        return false;
    }
    return std::unexpected(
        "expected true or false for " + field + " at line " +
        std::to_string(line));
}

std::expected<motion::Direction, std::string> direction(
    const std::string& value,
    std::size_t line) {
    if (value == "start") {
        return motion::Direction::Start;
    }
    if (value == "same") {
        return motion::Direction::Same;
    }
    if (value == "rise") {
        return motion::Direction::Rise;
    }
    if (value == "fall") {
        return motion::Direction::Fall;
    }
    return std::unexpected(
        "invalid branch direction at line " + std::to_string(line));
}

struct Pending {
    std::string name;
    std::string family;
    std::string implementation;
    std::string provenance;
    std::string kind;
    std::string packageFamily;
    std::string packageName;
    std::string source;
    std::string ghammaz;
    std::string extension;
    bool ordered{};
    bool sawOrdered{};
    std::vector<family::BranchSpec> branches;
    std::size_t beginLine{};
    std::size_t ghammazLine{};
    std::size_t extensionLine{};
};

std::expected<Record, std::string> finish(
    Pending pending,
    std::size_t line) {
    if (pending.name.empty() || pending.family.empty() ||
        pending.implementation.empty() || pending.provenance.empty()) {
        return std::unexpected(
            "package at line " + std::to_string(pending.beginLine) +
            " is missing name, family, implementation, or provenance");
    }
    if (pending.implementation != "complete" &&
        pending.implementation != "incomplete") {
        return std::unexpected(
            "invalid implementation for package " + pending.name +
            " at line " + std::to_string(line));
    }

    Record result{
        pending.name,
        pending.family,
        pending.implementation == "complete",
        pending.provenance,
        pending.kind,
        std::nullopt,
    };
    if (pending.kind != "family") {
        return result;
    }
    if (pending.packageName.empty()) {
        pending.packageName = pending.name;
    }
    if (pending.packageFamily.empty()) {
        pending.packageFamily = pending.family;
    }
    if (pending.source.empty()) {
        pending.source = pending.provenance;
    }
    if (pending.ghammaz.empty() || pending.extension.empty()) {
        return std::unexpected(
            "family package " + pending.name +
            " is missing ghammaz or extension");
    }
    const auto ghammaz = ratio(
        pending.ghammaz, "ghammaz", pending.ghammazLine);
    if (!ghammaz) {
        return std::unexpected(ghammaz.error());
    }
    const auto extension = ratio(
        pending.extension, "extension", pending.extensionLine);
    if (!extension) {
        return std::unexpected(extension.error());
    }
    result.specification = family::Spec{
        std::move(pending.packageName),
        std::move(pending.packageFamily),
        *ghammaz,
        *extension,
        std::move(pending.source),
        std::move(pending.branches),
        pending.ordered,
    };
    return result;
}

std::filesystem::path default_path() {
    if (const auto* configured = std::getenv("MQ_MAQAM_COLLECTION");
        configured != nullptr && *configured != '\0') {
        return configured;
    }
    return "theory/data/maqam/arabic.collection";
}

} // namespace

std::expected<Set, std::string> load(
    const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) {
        return std::unexpected("cannot open maqam collection: " + path.string());
    }

    Set result;
    Pending pending;
    bool inPackage = false;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        line = trim(std::move(line));
        if (line.empty() || line.front() == '#') {
            continue;
        }
        if (line == "[package]") {
            if (inPackage) {
                return std::unexpected(
                    "nested package at line " + std::to_string(lineNumber));
            }
            inPackage = true;
            pending = {};
            pending.beginLine = lineNumber;
            continue;
        }
        if (line == "[/package]") {
            if (!inPackage) {
                return std::unexpected(
                    "package terminator without package at line " +
                    std::to_string(lineNumber));
            }
            const auto record = finish(std::move(pending), lineNumber);
            if (!record) {
                return std::unexpected(record.error());
            }
            result.records.push_back(std::move(*record));
            inPackage = false;
            continue;
        }

        const auto separator = line.find('=');
        if (separator == std::string::npos) {
            return std::unexpected(
                "expected key=value at line " + std::to_string(lineNumber));
        }
        const auto key = trim(line.substr(0, separator));
        const auto value = trim(line.substr(separator + 1));
        if (!inPackage) {
            if (key == "collection") {
                result.identity = value;
            } else if (key == "version") {
                if (value != "1") {
                    return std::unexpected(
                        "unsupported maqam collection version at line " +
                        std::to_string(lineNumber));
                }
            } else {
                return std::unexpected(
                    "unknown collection field " + key + " at line " +
                    std::to_string(lineNumber));
            }
            continue;
        }

        if (key == "name") {
            pending.name = value;
        } else if (key == "family") {
            pending.family = value;
        } else if (key == "implementation") {
            pending.implementation = value;
        } else if (key == "provenance") {
            pending.provenance = value;
        } else if (key == "kind") {
            pending.kind = value;
        } else if (key == "package") {
            pending.packageName = value;
        } else if (key == "template") {
            pending.packageFamily = value;
        } else if (key == "source") {
            pending.source = value;
        } else if (key == "ghammaz") {
            pending.ghammaz = value;
            pending.ghammazLine = lineNumber;
        } else if (key == "extension") {
            pending.extension = value;
            pending.extensionLine = lineNumber;
        } else if (key == "ordered") {
            const auto parsed = boolean(value, key, lineNumber);
            if (!parsed) {
                return std::unexpected(parsed.error());
            }
            pending.ordered = *parsed;
            pending.sawOrdered = true;
        } else if (key == "branch") {
            const auto fields = split(value, '|');
            if (fields.size() != 8 || fields[0].empty() || fields[1].empty() ||
                fields[2].empty() || fields[7].empty()) {
                return std::unexpected(
                    "branch requires name|target|provenance|source-center|"
                    "center|tonic-role|ghammaz-role|direction at line " +
                    std::to_string(lineNumber));
            }
            const auto target = ratio(fields[1], "branch target", lineNumber);
            if (!target) {
                return std::unexpected(target.error());
            }
            const auto branchDirection = direction(fields[7], lineNumber);
            if (!branchDirection) {
                return std::unexpected(branchDirection.error());
            }
            pending.branches.emplace_back(
                fields[0],
                *target,
                fields[2],
                fields[3],
                fields[4],
                fields[5],
                fields[6],
                *branchDirection);
        } else {
            return std::unexpected(
                "unknown package field " + key + " at line " +
                std::to_string(lineNumber));
        }
    }
    if (inPackage) {
        return std::unexpected(
            "unterminated package at line " +
            std::to_string(pending.beginLine));
    }
    if (result.identity.empty()) {
        return std::unexpected("maqam collection has no identity");
    }
    if (result.records.empty()) {
        return std::unexpected("maqam collection has no packages");
    }
    return result;
}

std::expected<Set, std::string> load_default() {
    return load(default_path());
}

std::expected<family::Spec, std::string> find_spec(
    const Set& set,
    std::string_view name) {
    for (const auto& record : set.records) {
        if (record.name != name) {
            continue;
        }
        if (!record.specification) {
            return std::unexpected(
                "maqam collection record has no executable specification: " +
                std::string(name));
        }
        return *record.specification;
    }
    return std::unexpected(
        "maqam collection has no package: " + std::string(name));
}

std::expected<family::Spec, std::string> find_spec_default(
    std::string_view name) {
    const auto set = load_default();
    if (!set) {
        return std::unexpected(set.error());
    }
    return find_spec(*set, name);
}

} // namespace mq::kernel::maqam::collection
