#include "mq/kernel/validation/Casebook.hpp"

#include <algorithm>
#include <vector>

namespace mq::kernel::validation {
namespace {

bool contains(const std::vector<std::string>& values, const std::string& value) {
    return std::ranges::find(values, value) != values.end();
}

} // namespace

Report validate(const Casebook& casebook, std::string_view profile) {
    Report report;
    report.performance_count = casebook.performances.size();
    std::vector<std::string> performers;
    std::vector<std::string> recordings;
    std::vector<std::string> splits;
    std::vector<Rational> performer_totals;
    std::vector<std::string> total_performers;
    Rational total_duration{};
    Rational total_release{};
    Rational total_ornament{};
    std::size_t release_count{};
    std::size_t ornament_count{};
    std::size_t pause_count{};

    report.profile_covered = !profile.empty();
    for (const auto& performance : casebook.performances) {
        if (performance.profile != profile) {
            report.profile_covered = false;
            report.diagnostics.push_back(
                "profile mismatch in " + performance.identity);
        }
        if (!contains(performers, performance.performer)) {
            performers.push_back(performance.performer);
            total_performers.push_back(performance.performer);
            performer_totals.push_back(Rational{});
        }
        if (!contains(recordings, performance.recording)) {
            recordings.push_back(performance.recording);
        }
        if (!contains(splits, performance.split)) {
            splits.push_back(performance.split);
        }
        const auto performer_index = static_cast<std::size_t>(
            std::ranges::find(total_performers, performance.performer) -
            total_performers.begin());
        for (const auto& observation : performance.observations) {
            ++report.observation_count;
            total_duration += observation.duration;
            total_release += observation.release;
            total_ornament += observation.ornament;
            performer_totals[performer_index] += observation.duration;
            if (observation.release > Rational(0)) ++release_count;
            if (observation.ornament > Rational(0)) ++ornament_count;
        }
        pause_count += performance.pauses.size();
        if (!performance.well_formed()) {
            report.well_formed = false;
            report.diagnostics.push_back(
                "malformed evidence or observation in " + performance.identity);
        }
    }
    report.well_formed = casebook.well_formed() && report.observation_count > 0;
    report.performer_count = performers.size();
    report.recording_count = recordings.size();
    report.split_covered = contains(splits, "train") &&
                           contains(splits, "development") &&
                           contains(splits, "test");
    report.release_covered = release_count == report.observation_count;
    report.ornament_covered = ornament_count > 0;
    report.pauses_covered = pause_count > 0;
    if (performer_totals.size() > 1) {
        report.performer_variation = std::ranges::any_of(
            performer_totals,
            [&](const Rational& value) { return value != performer_totals.front(); });
    }
    if (report.observation_count > 0) {
        const auto count = Rational(static_cast<std::int64_t>(
            report.observation_count));
        report.mean_duration = total_duration / count;
        report.mean_release = total_release / count;
        report.mean_ornament_fraction = total_ornament / count;
    }
    report.release_ready =
        report.well_formed && report.profile_covered && report.split_covered &&
        report.performer_count >= 2 && report.recording_count >= 2 &&
        report.performer_variation && report.release_covered &&
        report.ornament_covered && report.pauses_covered;
    if (!report.release_ready) {
        report.diagnostics.push_back(
            "casebook does not meet the complete timing-validation gate");
    }
    return report;
}

} // namespace mq::kernel::validation
