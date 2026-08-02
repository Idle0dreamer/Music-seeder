#include "../Test.hpp"

#include "mq/kernel/validation/Casebook.hpp"

void test::validation::casebook() {
    const auto casebook = mq::kernel::validation::load_casebook(
        "theory/data/corpus/amicor-timing.casebook");
    require(casebook.has_value(), casebook.error_or("casebook failed"));
    const auto report = mq::kernel::validation::validate(
        *casebook,
        "arab-maqam.mawwal");
    require(report.well_formed, "corpus casebook was not well formed");
    require(report.profile_covered, "casebook profile coverage was lost");
    require(report.split_covered, "casebook split coverage was lost");
    require(report.performer_variation, "performer variation was not measured");
    require(report.release_covered, "release timing was not covered");
    require(report.ornament_covered, "ornament timing was not covered");
    require(report.pauses_covered, "phrase pauses were not covered");
    require(report.release_ready, "casebook did not pass timing validation");
}
