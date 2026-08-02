#pragma once

#include "mq/kernel/Rational.hpp"

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace mq::kernel::validation {

struct Evidence {
    std::string source;
    std::string location;
    std::string rights;
    std::string scope;
    std::string annotator;
    Rational confidence;
    Rational disagreement;

    [[nodiscard]] bool well_formed() const {
        return !source.empty() && !location.empty() && !rights.empty() &&
               !scope.empty() && !annotator.empty() &&
               confidence >= Rational(0) && confidence <= Rational(1) &&
               disagreement >= Rational(0) && disagreement <= Rational(1);
    }
};

struct Observation {
    Rational duration;
    Rational release;
    Rational ornament;

    [[nodiscard]] bool well_formed() const {
        return duration > Rational(0) && release >= Rational(0) &&
               ornament >= Rational(0) && ornament <= Rational(1);
    }
};

struct Performance {
    std::string identity;
    std::string profile;
    std::string performer;
    std::string recording;
    std::string split;
    std::vector<Observation> observations;
    std::vector<Rational> pauses;
    Evidence evidence;

    [[nodiscard]] bool well_formed() const {
        if (identity.empty() || profile.empty() || performer.empty() ||
            recording.empty() || split.empty() || observations.empty() ||
            !evidence.well_formed()) {
            return false;
        }
        for (const auto& observation : observations) {
            if (!observation.well_formed()) return false;
        }
        for (const auto& pause : pauses) {
            if (pause <= Rational(0)) return false;
        }
        return true;
    }
};

struct Casebook {
    std::string provenance;
    std::vector<Performance> performances;

    [[nodiscard]] bool well_formed() const {
        if (provenance.empty() || performances.empty()) return false;
        for (const auto& performance : performances) {
            if (!performance.well_formed()) return false;
        }
        return true;
    }
};

struct Report {
    bool well_formed{};
    bool profile_covered{};
    bool split_covered{};
    bool performer_variation{};
    bool release_covered{};
    bool ornament_covered{};
    bool pauses_covered{};
    bool release_ready{};
    std::size_t performance_count{};
    std::size_t observation_count{};
    std::size_t performer_count{};
    std::size_t recording_count{};
    Rational mean_duration{};
    Rational mean_release{};
    Rational mean_ornament_fraction{};
    std::vector<std::string> diagnostics;
};

[[nodiscard]] std::expected<Casebook, std::string> parse_casebook(
    std::string_view text);

[[nodiscard]] std::expected<Casebook, std::string> load_casebook(
    const std::filesystem::path& path);

[[nodiscard]] Report validate(
    const Casebook& casebook,
    std::string_view profile);

} // namespace mq::kernel::validation
