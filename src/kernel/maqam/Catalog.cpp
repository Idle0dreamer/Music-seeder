#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>

namespace mq::kernel::maqam {
namespace {

Identity id(const std::string& name) {
    return {"maqam.catalog", name, "1"};
}

Entry entry(
    std::string name,
    std::string family,
    Implementation implementation,
    std::string provenance,
    Builder builder = nullptr) {
    return {
        sort::MaqamId{id("maqam." + name)},
        sort::FamilyId{id("family." + family)},
        std::move(name),
        implementation,
        std::move(provenance),
        builder,
    };
}

} // namespace

Catalog Catalog::declared() {
    Catalog result;
    const auto source =
        "MaqamWorld:maqam-index;scope:canonical-arabic-repertoire-v1";
    result.entries_ = {
        entry("ajam", "ajam", Implementation::Complete, source, make_ajam),
        entry("ajam.ushayran", "ajam", Implementation::Incomplete, source),
        entry("athar.kurd", "nikriz", Implementation::Incomplete, source),
        entry("awj.iraq", "sikah", Implementation::Incomplete, source),
        entry("bastanikar", "sikah", Implementation::Incomplete, source),
        entry("bayati", "bayati", Implementation::Incomplete, source, make_bayati),
        entry("bayati.shuri", "bayati", Implementation::Incomplete, source),
        entry("dalanshin", "rast", Implementation::Incomplete, source),
        entry("farahfaza", "nahawand", Implementation::Incomplete, source),
        entry("hijaz", "hijaz", Implementation::Complete, source, make_hijaz),
        entry("hijazkar", "hijaz", Implementation::Incomplete, source),
        entry("hijazkar.kurd", "kurd", Implementation::Incomplete, source),
        entry("husayni", "bayati", Implementation::Incomplete, source),
        entry("huzam", "sikah", Implementation::Incomplete, source),
        entry("iraq", "sikah", Implementation::Incomplete, source),
        entry("jiharkah", "independent", Implementation::Incomplete, source),
        entry("kirdan", "rast", Implementation::Incomplete, source),
        entry("kurd", "kurd", Implementation::Complete, source, make_kurd),
        entry("lami", "independent", Implementation::Incomplete, source),
        entry("mahur", "rast", Implementation::Incomplete, source),
        entry("muhayyar", "bayati", Implementation::Incomplete, source),
        entry("musta.ar", "sikah", Implementation::Incomplete, source),
        entry("nahawand", "nahawand", Implementation::Complete, source, make_nahawand),
        entry("nahawand.murassa", "nahawand", Implementation::Incomplete, source),
        entry("nairuz", "rast", Implementation::Incomplete, source),
        entry("nawa.athar", "nikriz", Implementation::Incomplete, source),
        entry("nikriz", "nikriz", Implementation::Complete, source, make_nikriz),
        entry("rahat.al.arwah", "sikah", Implementation::Incomplete, source),
        entry("rast", "rast", Implementation::Complete, source, make_rast),
        entry("saba", "independent", Implementation::Incomplete, source),
        entry("saba.zamzam", "independent", Implementation::Incomplete, source),
        entry("sazkar", "rast", Implementation::Incomplete, source),
        entry("shadd.araban", "hijaz", Implementation::Incomplete, source),
        entry("shahnaz", "hijaz", Implementation::Incomplete, source),
        entry("shawq.afza", "ajam", Implementation::Incomplete, source),
        entry("sikah", "sikah", Implementation::Incomplete, source),
        entry("sikah.baladi", "independent", Implementation::Incomplete, source),
        entry("suzdalara", "rast", Implementation::Incomplete, source),
        entry("suzidil", "hijaz", Implementation::Incomplete, source),
        entry("suznak", "rast", Implementation::Incomplete, source),
        entry("ushaq.masri", "nahawand", Implementation::Incomplete, source),
        entry("yakah", "rast", Implementation::Incomplete, source),
        entry("zanjaran", "hijaz", Implementation::Incomplete, source),
    };
    return result;
}

const std::vector<Entry>& Catalog::entries() const noexcept {
    return entries_;
}

const Entry* Catalog::find(const sort::MaqamId& id) const noexcept {
    const auto found = std::ranges::find(entries_, id, &Entry::id);
    return found == entries_.end() ? nullptr : &*found;
}

std::expected<Scaffold, std::string> Catalog::build(
    const sort::MaqamId& id) const {
    const auto* found = find(id);
    if (found == nullptr) {
        return std::unexpected("maqam is not declared in the catalog");
    }
    if (found->implementation != Implementation::Complete) {
        return std::unexpected(
            "maqam package is declared but not complete: " + found->name);
    }
    if (found->builder == nullptr) {
        return std::unexpected("maqam package has no builder: " + found->name);
    }
    return found->builder();
}

} // namespace mq::kernel::maqam
