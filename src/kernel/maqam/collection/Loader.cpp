#include "mq/kernel/maqam/collection/Loader.hpp"

#include "mq/kernel/motion/Direction.hpp"

#include <algorithm>
#include <array>
#include <charconv>
#include <cstdlib>
#include <fstream>
#include <set>
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

std::vector<std::string> list(const std::string& value) {
    return value == "-" ? std::vector<std::string>{} : split(value, ',');
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

std::expected<Rational, std::string> rational(
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
    return Rational(*numerator, *denominator);
}

std::expected<pitch::Expression, std::string> ratio(
    const std::string& value,
    const std::string& field,
    std::size_t line) {
    const auto parsed = rational(value, field, line);
    if (!parsed) {
        return std::unexpected(parsed.error());
    }
    return pitch::Expression::ratio(
        parsed->numerator(), parsed->denominator());
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
    std::string upperRole;
    std::vector<std::string> rootRoles;
    std::vector<family::BranchSpec> branches;
    std::vector<family::RouteSpec> routes;
    std::vector<family::AuthoritySpec> authorities;
    std::vector<family::JinsSpec> jins;
    std::vector<family::GestureSpec> gestures;
    std::vector<family::BaggageSpec> baggage;
    std::vector<family::ObligationSpec> obligations;
    std::vector<family::FormulaSpec> formulas;
    std::vector<family::FormulaVariationSpec> formulaVariations;
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
    if (pending.routes.empty()) {
        return std::unexpected(
            "family package " + pending.name +
            " has no declared routes");
    }
    for (const auto& authority : pending.authorities) {
        if (authority.kind.empty() || authority.names.empty() ||
            std::ranges::any_of(authority.names, [](const auto& name) {
                return name.empty();
            })) {
            return std::unexpected(
                "authority declarations must contain a kind and names for " +
                pending.name);
        }
    }
    std::set<std::string> authorityNames;
    for (const auto& authority : pending.authorities) {
        for (const auto& name : authority.names) {
            const auto token = authority.kind + "." + name;
            if (!authorityNames.insert(token).second) {
                return std::unexpected(
                    "duplicate authority declaration " + token + " for " +
                    pending.name);
            }
        }
    }
    for (const auto& route : pending.routes) {
        if (route.steps.empty()) {
            return std::unexpected(
                "route " + route.name + " has no declared steps");
        }
        std::set<std::string> stepNames;
        for (const auto& step : route.steps) {
            if (!stepNames.insert(step.name).second) {
                return std::unexpected(
                    "route " + route.name + " repeats step " + step.name);
            }
        }
        for (const auto& step : route.steps) {
            if (step.minimum > step.maximum) {
                return std::unexpected(
                    "route " + route.name + " step " + step.name +
                    " declares an invalid repetition range");
            }
            if (step.next) {
                for (const auto& next : *step.next) {
                    if (!stepNames.contains(next)) {
                        return std::unexpected(
                            "route " + route.name + " step " + step.name +
                            " references unknown next step " + next);
                    }
                }
            }
            for (const auto& action : step.actions) {
                if (action.variant && *action.variant >= route.variants) {
                    return std::unexpected(
                        "route " + route.name + " action variant is outside "
                        "the declared route variant set");
                }
            }
        }
    }
    const auto validate_unique = [&](const auto& values,
                                     auto identity,
                                     const std::string& kind)
        -> std::expected<void, std::string> {
        std::set<std::string> names;
        for (const auto& value : values) {
            const auto name = identity(value);
            if (!names.insert(name).second) {
                return std::unexpected(
                    "duplicate " + kind + " declaration " + name +
                    " for " + pending.name);
            }
        }
        return {};
    };
    if (const auto checked = validate_unique(
            pending.jins,
            [](const auto& value) { return value.name; },
            "jins");
        !checked) {
        return std::unexpected(checked.error());
    }
    if (const auto checked = validate_unique(
            pending.gestures,
            [](const auto& value) { return value.owner + "." + value.name; },
            "gesture");
        !checked) {
        return std::unexpected(checked.error());
    }
    if (const auto checked = validate_unique(
            pending.baggage,
            [](const auto& value) { return value.owner + "." + value.name; },
            "baggage");
        !checked) {
        return std::unexpected(checked.error());
    }
    if (const auto checked = validate_unique(
            pending.obligations,
            [](const auto& value) { return value.name; },
            "obligation");
        !checked) {
        return std::unexpected(checked.error());
    }
    if (const auto checked = validate_unique(
            pending.formulas,
            [](const auto& value) { return value.name; },
            "formula");
        !checked) {
        return std::unexpected(checked.error());
    }
    std::set<std::string> variationPairs;
    for (const auto& variation : pending.formulaVariations) {
        if (variation.base.empty() || variation.variation.empty() ||
            variation.transformation.empty() || variation.provenance.empty()) {
            return std::unexpected(
                "formula variation requires base, variation, transformation, "
                "and provenance for " + pending.name);
        }
        const auto pair = variation.base + "->" + variation.variation;
        if (!variationPairs.insert(pair).second) {
            return std::unexpected(
                "duplicate formula variation " + pair + " for " +
                pending.name);
        }
        const auto base = std::ranges::find_if(
            pending.formulas,
            [&](const auto& formula) { return formula.name == variation.base; });
        const auto changed = std::ranges::find_if(
            pending.formulas,
            [&](const auto& formula) {
                return formula.name == variation.variation;
            });
        if (base == pending.formulas.end() || changed == pending.formulas.end()) {
            return std::unexpected(
                "formula variation references an undeclared formula " + pair +
                " for " + pending.name);
        }
        if (base->cell != changed->cell) {
            return std::unexpected(
                "formula variation changes cell authority " + pair +
                " for " + pending.name);
        }
    }
    for (const auto& formula : pending.formulas) {
        if (formula.cell.empty() || formula.provenance.empty() ||
            formula.notes.empty()) {
            return std::unexpected(
                "formula declarations require cell, provenance, and notes "
                "for " + pending.name);
        }
        if (!authorityNames.contains("cell." + formula.cell)) {
            return std::unexpected(
                "formula references an undeclared cell authority " +
                formula.cell + " in " + pending.name);
        }
        for (const auto& note : formula.notes) {
            if (note.event.empty() || note.role.empty() ||
                note.direction.empty() || note.region.empty() ||
                note.emphasis < Rational(0) || note.dwell < Rational(0)) {
                return std::unexpected(
                    "formula note is incomplete or has negative evidence "
                    "for " + formula.name + " in " + pending.name);
            }
        }
    }
    if (pending.implementation == "complete") {
        for (const auto& authority : pending.authorities) {
            if (authority.kind != "formula") continue;
            for (const auto& name : authority.names) {
                const auto declaredFormula = std::ranges::find_if(
                    pending.formulas,
                    [&](const auto& formula) { return formula.name == name; });
                if (declaredFormula == pending.formulas.end()) {
                    return std::unexpected(
                        "complete package formula authority has no declared "
                        "surface: " + name + " in " + pending.name);
                }
            }
        }
    }
    result.specification = family::Spec{
        .package = std::move(pending.packageName),
        .family = std::move(pending.packageFamily),
        .ghammaz = *ghammaz,
        .extension = *extension,
        .upper_role = std::move(pending.upperRole),
        .provenance = std::move(pending.source),
        .branches = std::move(pending.branches),
        .root_roles = std::move(pending.rootRoles),
        .authorities = std::move(pending.authorities),
        .jins = std::move(pending.jins),
        .gestures = std::move(pending.gestures),
        .baggage = std::move(pending.baggage),
        .obligations = std::move(pending.obligations),
        .formulas = std::move(pending.formulas),
        .formula_variations = std::move(pending.formulaVariations),
        .routes = std::move(pending.routes),
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
        } else if (key == "upper-role") {
            pending.upperRole = value;
        } else if (key == "root-roles") {
            const auto fields = split(value, ',');
            if (fields.empty() ||
                std::ranges::any_of(fields, [](const auto& field) {
                    return field.empty();
                })) {
                return std::unexpected(
                    "root-roles requires comma-separated names at line " +
                    std::to_string(lineNumber));
            }
            pending.rootRoles = fields;
        } else if (key == "jins") {
            const auto fields = split(value, '|');
            if (fields.size() != 11 || fields[0].empty() ||
                fields[1].empty()) {
                return std::unexpected(
                    "jins requires name|tonic|roles|ghammaz|regions|"
                    "characteristic|emphasis|entry|exit|cadences|motifs at line " +
                    std::to_string(lineNumber));
            }
            const auto values = std::array<std::vector<std::string>, 9>{
                list(fields[2]),
                list(fields[3]),
                list(fields[4]),
                list(fields[5]),
                list(fields[6]),
                list(fields[7]),
                list(fields[8]),
                list(fields[9]),
                list(fields[10]),
            };
            if (std::ranges::any_of(
                    values,
                    [](const auto& names) {
                        return std::ranges::any_of(
                            names,
                            [](const auto& name) { return name.empty(); });
                    })) {
                return std::unexpected(
                    "jins contains an empty list name at line " +
                    std::to_string(lineNumber));
            }
            pending.jins.push_back({
                fields[0],
                fields[1],
                values[0],
                values[1],
                values[2],
                values[3],
                values[4],
                values[5],
                values[6],
                values[7],
                values[8],
            });
        } else if (key == "gesture") {
            const auto fields = split(value, '|');
            if (fields.size() != 3 || fields[0].empty() ||
                fields[1].empty() || fields[2] == "-") {
                return std::unexpected(
                    "gesture requires owner|name|step-list at line " +
                    std::to_string(lineNumber));
            }
            std::vector<family::GestureStepSpec> steps;
            for (const auto& rawStep : split(fields[2], ';')) {
                const auto step = split(rawStep, ',');
                if (step.size() != 4 || step[0].empty() ||
                    step[1].empty() || step[2].empty() || step[3].empty()) {
                    return std::unexpected(
                        "gesture step requires roles,regions,direction,"
                        "baggage at line " + std::to_string(lineNumber));
                }
                const auto stepDirection = direction(step[2], lineNumber);
                if (!stepDirection) {
                    return std::unexpected(stepDirection.error());
                }
                const auto roles = list(step[0]);
                const auto regions = list(step[1]);
                if (roles.empty() || regions.empty() ||
                    roles.size() != regions.size()) {
                    return std::unexpected(
                        "gesture step roles and regions must be nonempty and "
                        "parallel at line " + std::to_string(lineNumber));
                }
                steps.push_back({
                    roles,
                    regions,
                    *stepDirection,
                    step[3] == "-"
                        ? std::nullopt
                        : std::optional<std::string>{step[3]},
                });
            }
            pending.gestures.push_back({
                fields[0], fields[1], std::move(steps)});
        } else if (key == "baggage") {
            const auto fields = split(value, '|');
            if (fields.size() != 6 || fields[0].empty() ||
                fields[1].empty() || fields[2].empty() ||
                fields[3].empty() || fields[4].empty() ||
                fields[5].empty()) {
                return std::unexpected(
                    "baggage requires owner|name|role|regions|directions|"
                    "gestures at line " + std::to_string(lineNumber));
            }
            const auto regions = list(fields[3]);
            const auto directionNames = list(fields[4]);
            const auto gestures = list(fields[5]);
            if (regions.empty() || directionNames.empty() || gestures.empty()) {
                return std::unexpected(
                    "baggage requires nonempty regions, directions, and "
                    "gestures at line " + std::to_string(lineNumber));
            }
            std::vector<motion::Direction> directions;
            for (const auto& name : directionNames) {
                const auto parsed = direction(name, lineNumber);
                if (!parsed) return std::unexpected(parsed.error());
                directions.push_back(*parsed);
            }
            pending.baggage.push_back({
                fields[0],
                fields[1],
                fields[2],
                regions,
                std::move(directions),
                gestures,
            });
        } else if (key == "sayr-obligation") {
            const auto fields = split(value, '|');
            if (fields.size() != 3 || fields[0].empty() ||
                fields[1].empty() || fields[2].empty()) {
                return std::unexpected(
                    "sayr-obligation requires name|after|needs at line " +
                    std::to_string(lineNumber));
            }
            std::vector<family::NeedSpec> needs;
            if (fields[2] != "-") {
                for (const auto& rawNeed : split(fields[2], ';')) {
                    const auto parts = split(rawNeed, ':');
                    if (parts.size() < 2 || parts[0].empty()) {
                        return std::unexpected(
                            "sayr need requires kind:arguments at line " +
                            std::to_string(lineNumber));
                    }
                    std::vector<std::string> arguments(parts.begin() + 1, parts.end());
                    if (std::ranges::any_of(
                            arguments,
                            [](const auto& argument) {
                                return argument.empty();
                            })) {
                        return std::unexpected(
                            "sayr need contains an empty argument at line " +
                            std::to_string(lineNumber));
                    }
                    needs.push_back({parts[0], std::move(arguments)});
                }
            }
            pending.obligations.push_back({
                fields[0], list(fields[1]), std::move(needs)});
        } else if (key == "formula") {
            const auto fields = split(value, '|');
            if (fields.size() != 4 || fields[0].empty() ||
                fields[1].empty() || fields[2].empty() ||
                fields[3] == "-") {
                return std::unexpected(
                    "formula requires name|cell|provenance|note-list at line " +
                    std::to_string(lineNumber));
            }
            std::vector<family::FormulaNoteSpec> notes;
            for (const auto& rawNote : split(fields[3], ';')) {
                const auto note = split(rawNote, ',');
                if (note.size() != 7 || note[0].empty() || note[1].empty() ||
                    note[2].empty() || note[3].empty() || note[4].empty()) {
                    return std::unexpected(
                        "formula note requires event,role,direction,region,"
                        "baggage,emphasis,dwell at line " +
                        std::to_string(lineNumber));
                }
                const auto emphasis = rational(
                    note[5], "formula emphasis", lineNumber);
                const auto dwell = rational(
                    note[6], "formula dwell", lineNumber);
                if (!emphasis) return std::unexpected(emphasis.error());
                if (!dwell) return std::unexpected(dwell.error());
                family::FormulaNoteSpec parsed;
                parsed.event = note[0];
                parsed.role = note[1];
                parsed.direction = note[2];
                parsed.region = note[3];
                if (note[4] != "-") {
                    parsed.baggage = note[4];
                }
                parsed.emphasis = *emphasis;
                parsed.dwell = *dwell;
                notes.push_back(std::move(parsed));
            }
            pending.formulas.push_back({
                fields[0], fields[1], fields[2], std::move(notes)});
        } else if (key == "formula-variation") {
            const auto fields = split(value, '|');
            if (fields.size() != 4 || fields[0].empty() ||
                fields[1].empty() || fields[2].empty() || fields[3].empty()) {
                return std::unexpected(
                    "formula-variation requires base|variation|"
                    "transformation|provenance at line " +
                    std::to_string(lineNumber));
            }
            pending.formulaVariations.push_back({
                fields[0], fields[1], fields[2], fields[3]});
        } else if (key == "authority") {
            const auto fields = split(value, '|');
            if (fields.size() != 2 || fields[0].empty() || fields[1].empty()) {
                return std::unexpected(
                    "authority requires kind|comma-separated-names at line " +
                    std::to_string(lineNumber));
            }
            const auto names = split(fields[1], ',');
            if (names.empty() ||
                std::ranges::any_of(names, [](const auto& name) {
                    return name.empty();
                })) {
                return std::unexpected(
                    "authority contains an empty name at line " +
                    std::to_string(lineNumber));
            }
            pending.authorities.push_back({fields[0], names});
        } else if (key == "route") {
            const auto fields = split(value, '|');
            if (fields.size() != 2 || fields[0].empty()) {
                return std::unexpected(
                    "route requires name|branches at line " +
                    std::to_string(lineNumber));
            }
            family::RouteSpec route{fields[0], {}, {}, 1, {}};
            if (!fields[1].empty()) {
                route.branches = split(fields[1], ',');
                if (std::ranges::any_of(
                        route.branches,
                        [](const auto& branch) { return branch.empty(); })) {
                    return std::unexpected(
                        "route contains an empty branch name at line " +
                        std::to_string(lineNumber));
                }
            }
            pending.routes.push_back(std::move(route));
        } else if (key == "step") {
            const auto fields = split(value, '|');
            if (fields.size() != 4 || fields[0].empty() ||
                fields[1].empty() || fields[2].empty() || fields[3].empty()) {
                return std::unexpected(
                    "step requires route|name|branch-or-root|actions at line " +
                    std::to_string(lineNumber));
            }
            const auto found = std::ranges::find_if(
                pending.routes,
                [&](const auto& route) { return route.name == fields[0]; });
            if (found == pending.routes.end()) {
                return std::unexpected(
                    "step references unknown route at line " +
                    std::to_string(lineNumber));
            }
            const auto actionFields = split(fields[3], ';');
            if (actionFields.empty()) {
                return std::unexpected(
                    "step must declare at least one action at line " +
                    std::to_string(lineNumber));
            }
            std::vector<family::ActionSpec> actions;
            for (const auto& action : actionFields) {
                const auto separator = action.find(':');
                if (separator == std::string::npos || separator == 0 ||
                    separator + 1 >= action.size()) {
                    return std::unexpected(
                        "step action requires operation:arguments at line " +
                        std::to_string(lineNumber));
                }
                auto operation = trim(action.substr(0, separator));
                std::optional<std::size_t> variant;
                const auto marker = operation.rfind('@');
                if (marker != std::string::npos) {
                    if (marker == 0 || marker + 1 >= operation.size()) {
                        return std::unexpected(
                            "variant-qualified action requires operation@variant at line " +
                            std::to_string(lineNumber));
                    }
                    const auto parsed = integer(
                        operation.substr(marker + 1),
                        "action variant",
                        lineNumber);
                    if (!parsed || *parsed < 0) {
                        return std::unexpected(
                            parsed ? "action variant must be non-negative at line " +
                                          std::to_string(lineNumber)
                                   : parsed.error());
                    }
                    variant = static_cast<std::size_t>(*parsed);
                    operation.resize(marker);
                }
                actions.push_back({
                    std::move(operation),
                    split(action.substr(separator + 1), ','),
                    variant});
            }
            found->steps.push_back({
                fields[1],
                fields[2] == "-" ? std::string{} : fields[2],
                std::move(actions),
                std::nullopt});
        } else if (key == "step-repeat") {
            const auto fields = split(value, '|');
            if (fields.size() != 4 || fields[0].empty() ||
                fields[1].empty() || fields[2].empty() || fields[3].empty()) {
                return std::unexpected(
                    "step-repeat requires route|step|min|max at line " +
                    std::to_string(lineNumber));
            }
            const auto minimum = integer(
                fields[2], "step-repeat minimum", lineNumber);
            const auto maximum = integer(
                fields[3], "step-repeat maximum", lineNumber);
            if (!minimum) return std::unexpected(minimum.error());
            if (!maximum) return std::unexpected(maximum.error());
            if (*minimum < 0 || *maximum < 0 || *minimum > *maximum) {
                return std::unexpected(
                    "step-repeat requires a nonnegative min <= max at line " +
                    std::to_string(lineNumber));
            }
            const auto foundRoute = std::ranges::find_if(
                pending.routes,
                [&](const auto& route) { return route.name == fields[0]; });
            if (foundRoute == pending.routes.end()) {
                return std::unexpected(
                    "step-repeat references unknown route at line " +
                    std::to_string(lineNumber));
            }
            const auto foundStep = std::ranges::find_if(
                foundRoute->steps,
                [&](const auto& step) { return step.name == fields[1]; });
            if (foundStep == foundRoute->steps.end()) {
                return std::unexpected(
                    "step-repeat references unknown step at line " +
                    std::to_string(lineNumber));
            }
            if (foundStep->minimum != 1 || foundStep->maximum != 1) {
                return std::unexpected(
                    "step-repeat repeats a step declaration at line " +
                    std::to_string(lineNumber));
            }
            foundStep->minimum = static_cast<std::size_t>(*minimum);
            foundStep->maximum = static_cast<std::size_t>(*maximum);
        } else if (key == "step-next") {
            const auto fields = split(value, '|');
            if (fields.size() != 3 || fields[0].empty() ||
                fields[1].empty() || fields[2].empty()) {
                return std::unexpected(
                    "step-next requires route|step|comma-separated-next-"
                    "steps at line " + std::to_string(lineNumber));
            }
            const auto foundRoute = std::ranges::find_if(
                pending.routes,
                [&](const auto& route) { return route.name == fields[0]; });
            if (foundRoute == pending.routes.end()) {
                return std::unexpected(
                    "step-next references unknown route at line " +
                    std::to_string(lineNumber));
            }
            const auto foundStep = std::ranges::find_if(
                foundRoute->steps,
                [&](const auto& step) { return step.name == fields[1]; });
            if (foundStep == foundRoute->steps.end()) {
                return std::unexpected(
                    "step-next references unknown step at line " +
                    std::to_string(lineNumber));
            }
            if (foundStep->next) {
                return std::unexpected(
                    "step-next repeats a step declaration at line " +
                    std::to_string(lineNumber));
            }
            if (fields[2] == "-") {
                foundStep->next = std::vector<std::string>{};
            } else {
                const auto next = split(fields[2], ',');
                if (next.empty() || std::ranges::any_of(
                        next, [](const auto& name) { return name.empty(); })) {
                    return std::unexpected(
                        "step-next contains an empty next step at line " +
                        std::to_string(lineNumber));
                }
                foundStep->next = next;
            }
        } else if (key == "route-variants") {
            if (pending.routes.empty()) {
                return std::unexpected(
                    "route-variants must follow route at line " +
                    std::to_string(lineNumber));
            }
            const auto parsed = integer(value, key, lineNumber);
            if (!parsed || *parsed <= 0) {
                return std::unexpected(
                    parsed ? "route-variants must be positive at line " +
                                  std::to_string(lineNumber)
                           : parsed.error());
            }
            pending.routes.back().variants = static_cast<std::size_t>(*parsed);
        } else if (key == "route-terminals") {
            const auto fields = split(value, '|');
            if (fields.size() != 2 || fields[0].empty() || fields[1].empty()) {
                return std::unexpected(
                    "route-terminals requires route|comma-separated-"
                    "obligations at line " + std::to_string(lineNumber));
            }
            const auto found = std::ranges::find_if(
                pending.routes,
                [&](const auto& route) { return route.name == fields[0]; });
            if (found == pending.routes.end()) {
                return std::unexpected(
                    "route-terminals references unknown route at line " +
                    std::to_string(lineNumber));
            }
            if (!found->terminals.empty()) {
                return std::unexpected(
                    "route-terminals repeats a route at line " +
                    std::to_string(lineNumber));
            }
            found->terminals = list(fields[1]);
            if (found->terminals.empty() ||
                std::ranges::any_of(
                    found->terminals,
                    [](const auto& terminal) { return terminal.empty(); })) {
                return std::unexpected(
                    "route-terminals contains an empty obligation at line " +
                    std::to_string(lineNumber));
            }
        } else if (key == "ordered") {
            return std::unexpected(
                "ordered is obsolete; declare route and step records at line " +
                std::to_string(lineNumber));
        } else if (key == "branch") {
            const auto fields = split(value, '|');
            if ((fields.size() != 8 && fields.size() != 9) ||
                fields[0].empty() || fields[1].empty() ||
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
                *branchDirection,
                fields.size() == 9 ? fields[8] : std::string{});
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
