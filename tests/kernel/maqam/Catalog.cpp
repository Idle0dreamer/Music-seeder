#include "../Test.hpp"

#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>

void test::maqam_catalog() {
    using namespace mq::kernel;
    const auto collection = maqam::collection::load_default();
    require(
        collection.has_value(),
        collection.error_or("maqam collection failed to load"));
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
    const auto nahawand = catalog.find(sort::MaqamId{
        Identity{"maqam.catalog", "maqam.nahawand", "1"}});
    require(nahawand != nullptr, "Nahawand is missing from maqam catalog");
    require(
        nahawand->implementation == maqam::Implementation::Complete,
        "implemented Nahawand package was not marked complete");
    const auto nahawandPackage = catalog.build(nahawand->id);
    require(
        nahawandPackage.has_value(),
        nahawandPackage.error_or("Nahawand build failed"));
    for (const auto& name : {"ajam", "hijaz", "kurd", "nikriz"}) {
        const auto* package = catalog.find(sort::MaqamId{
            Identity{"maqam.catalog", "maqam." + std::string(name), "1"}});
        require(package != nullptr, std::string(name) + " is missing from maqam catalog");
        require(
            package->implementation == maqam::Implementation::Complete,
            std::string("implemented ") + name +
                " package was not marked complete");
        const auto built = catalog.build(package->id);
        require(built.has_value(), built.error_or(std::string(name) + " build failed"));
    }
    const auto* sikah = catalog.find(sort::MaqamId{
        Identity{"maqam.catalog", "maqam.sikah", "1"}});
    require(sikah != nullptr, "Sikah is missing from maqam catalog");
    require(
        sikah->implementation == maqam::Implementation::Incomplete,
        "Sikah vertical route was marked complete before package validation");
    const auto sikahPackage = catalog.build(sikah->id);
    require(
        !sikahPackage &&
            sikahPackage.error().find("not complete") != std::string::npos,
        "Sikah package bypassed the completion gate");
    const auto missing = catalog.build(sort::MaqamId{
        Identity{"maqam.catalog", "maqam.unknown", "1"}});
    require(
        !missing && missing.error().find("not declared") != std::string::npos,
        "undeclared maqam was accepted");
}
