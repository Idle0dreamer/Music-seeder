#include "mq/kernel/maqam/Bayati.hpp"
#include "mq/kernel/operation/Operation.hpp"

namespace mq::kernel::maqam {

namespace {

Identity id(const std::string& name) {
    return {"bayati", name, "1"};
}

std::shared_ptr<profile::Set> build_profile() {
    profile::Set base("bayati.base");
    
    // Detailed profile reconstruction based on Egyptian/Levantine practice
    // E.g., tonic Dukah (D), Ghammaz Nawa (G).
    // Intonation trajectory: E-half-flat has flexible tuning (approx -40 to -60 cents from E)
    // varying dynamically based on direction.
    const auto p = profile::reconstruct(
        "bayati.reconstruct",
        {base},
        {
            {profile::Patch::Action::Define, "bayati.role.tonic", profile::Rule{profile::Ordering{id("role.dukah")}, {"bayati.md", "research"}}},
            {profile::Patch::Action::Define, "bayati.role.ghammaz", profile::Rule{profile::Ordering{id("role.nawa")}, {"bayati.md", "research"}}},
            // We use the continuous trajectory basis for the E half flat.
            {profile::Patch::Action::Define, "bayati.intonation.sikah", profile::Rule{Rational(-50, 100), {"bayati.md", "research"}}}, // Approximate baseline
        }
    );
    if (!p) {
        return std::unexpected(p.error());
    }
    return std::make_shared<profile::Set>(*p);
}

std::expected<jins::Catalog, std::string> build_ajnas() {
    jins::Catalog catalog;
    // Jins Bayati on D
    jins::Descriptor bayati{
        id("jins.bayati"),
        id("role.dukah"), // tonic
        {id("role.dukah"), id("role.sikah"), id("role.jaharkah"), id("role.nawa")}, // span
        {id("role.nawa")}, // ghammaz
        {id("region.middle")},
        {id("region.middle")},
        {id("role.dukah"), id("role.nawa")}, // emphasis
        {}, // entry
        {}, // exit
        {}, // cadences
        {}, // motifs
        {}, // baggage
        {}, // gestures
        {}  // provenance
    };
    if (auto r = catalog.add(bayati); !r) return std::unexpected(r.error());
    
    // Jins Nahawand on G (Nawa)
    jins::Descriptor nahawand{
        id("jins.nahawand"),
        id("role.nawa"),
        {id("role.nawa"), id("role.husayni"), id("role.ajam"), id("role.kardan")},
        {id("role.kardan")},
        {id("region.middle")},
        {id("region.middle")},
        {id("role.nawa")},
        {}, {}, {}, {}, {}, {}, {}
    };
    if (auto r = catalog.add(nahawand); !r) return std::unexpected(r.error());

    // Jins Rast on G (Nawa)
    jins::Descriptor rast{
        id("jins.rast"),
        id("role.nawa"),
        {id("role.nawa"), id("role.husayni"), id("role.awj"), id("role.kardan")},
        {id("role.kardan")},
        {id("region.middle")},
        {id("region.middle")},
        {id("role.nawa")},
        {}, {}, {}, {}, {}, {}, {}
    };
    if (auto r = catalog.add(rast); !r) return std::unexpected(r.error());

    return catalog;
}

std::expected<path::Graph, std::string> build_graph() {
    path::Graph graph;
    // Direct valid transitions for Bayati
    auto r1 = graph.add(path::Rule{id("path.bayati_to_nahawand"), id("jins.bayati"), id("jins.nahawand"), tonicization::Level::Internal, {}, "research"});
    if (!r1) return std::unexpected(r1.error());
    
    auto r2 = graph.add(path::Rule{id("path.bayati_to_rast"), id("jins.bayati"), id("jins.rast"), tonicization::Level::Internal, {}, "research"});
    if (!r2) return std::unexpected(r2.error());
    
    return graph;
}

std::expected<sayr::Plan, std::string> build_sayr() {
    return std::unexpected("missing sourced sayr route for Bayati");
}

} // namespace

std::expected<Scaffold, std::string> make_bayati() {
    Scaffold scaffold;
    
    auto profile = build_profile();
    if (!profile) return std::unexpected("failed to build profile");
    scaffold.profile = profile;
    
    auto ajnas = build_ajnas();
    if (!ajnas) return std::unexpected(ajnas.error());
    scaffold.ajnas = *ajnas;
    
    auto graph = build_graph();
    if (!graph) return std::unexpected(graph.error());
    scaffold.graph = *graph;
    
    auto plan = build_sayr();
    if (!plan) return std::unexpected(plan.error());
    scaffold.sayr = *plan;

    return scaffold;
}

} // namespace mq::kernel::maqam
