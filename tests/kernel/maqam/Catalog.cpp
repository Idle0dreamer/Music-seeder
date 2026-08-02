#include "../Test.hpp"

#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>

void test::maqam_catalog() {
    using namespace mq::kernel;
    const auto catalog = maqam::Catalog::declared();
    require(catalog.entries().size() == 43, "maqam scope count changed");
    const auto bayati = catalog.find(sort::MaqamId{
        Identity{"maqam.catalog", "maqam.bayati", "1"}});
    require(bayati != nullptr, "Bayati is missing from maqam catalog");
    require(
        bayati->implementation == maqam::Implementation::Incomplete,
        "unfinished Bayati package was marked complete");
    const auto rejected = catalog.build(bayati->id);
    require(
        !rejected && rejected.error().find("not complete") != std::string::npos,
        "incomplete maqam package was executable");
    const auto rast = catalog.find(sort::MaqamId{
        Identity{"maqam.catalog", "maqam.rast", "1"}});
    require(rast != nullptr, "Rast is missing from maqam catalog");
    require(
        rast->implementation == maqam::Implementation::Complete,
        "implemented Rast package was not marked complete");
    const auto rastPackage = catalog.build(rast->id);
    require(rastPackage.has_value(), rastPackage.error_or("Rast build failed"));
    const auto missing = catalog.build(sort::MaqamId{
        Identity{"maqam.catalog", "maqam.unknown", "1"}});
    require(
        !missing && missing.error().find("not declared") != std::string::npos,
        "undeclared maqam was accepted");
}
