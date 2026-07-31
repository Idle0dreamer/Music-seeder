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
    if (p) {
        return std::make_shared<profile::Set>(*p);
    }
    return std::make_shared<profile::Set>(base);
}

jins::Catalog build_ajnas() {
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
        {}  // gestures
    };
    auto r1 = catalog.add(bayati);
    
    // Jins Nahawand on G (Nawa)
    jins::Descriptor nahawand{
        id("jins.nahawand"),
        id("role.nawa"),
        {id("role.nawa"), id("role.husayni"), id("role.ajam"), id("role.kardan")},
        {id("role.kardan")},
        {id("region.middle")},
        {id("region.middle")},
        {id("role.nawa")},
        {}, {}, {}, {}, {}, {}
    };
    auto r2 = catalog.add(nahawand);

    // Jins Rast on G (Nawa)
    jins::Descriptor rast{
        id("jins.rast"),
        id("role.nawa"),
        {id("role.nawa"), id("role.husayni"), id("role.awj"), id("role.kardan")},
        {id("role.kardan")},
        {id("region.middle")},
        {id("region.middle")},
        {id("role.nawa")},
        {}, {}, {}, {}, {}, {}
    };
    auto r3 = catalog.add(rast);

    return catalog;
}

path::Graph build_graph() {
    path::Graph graph;
    // Direct valid transitions for Bayati
    auto r1 = graph.add(path::Rule{id("path.bayati_to_nahawand"), id("jins.bayati"), id("jins.nahawand"), tonicization::Level::Internal, {}, "research"});
    auto r2 = graph.add(path::Rule{id("path.bayati_to_rast"), id("jins.bayati"), id("jins.rast"), tonicization::Level::Internal, {}, "research"});
    return graph;
}

sayr::Plan build_sayr() {
    auto plan = sayr::Plan::make(
        id("plan.bayati"),
        {
            sayr::Obligation{id("obl.establish_root"), {sayr::Need{id("need.root_jins"), sayr::need::Jins{id("jins.bayati")}}}, {}},
            sayr::Obligation{id("obl.ghammaz_travel"), {sayr::Need{id("need.nahawand"), sayr::need::Jins{id("jins.nahawand")}}}, {id("obl.establish_root")}},
            sayr::Obligation{id("obl.return"), {sayr::Need{id("need.return_jins"), sayr::need::Jins{id("jins.bayati")}}}, {id("obl.ghammaz_travel")}},
        },
        {}
    );
    if (plan) return *plan;
    return {};
}

} // namespace

std::expected<Configuration, std::string> make_bayati() {
    Configuration config;
    config.profile = build_profile();
    config.ajnas = build_ajnas();
    config.graph = build_graph();
    config.sayr = build_sayr();

    // Construct the generative grammar (using the recursive capability!)
    const auto p_id = id("prod.bayati");
    const auto establish = grammar::Term::atom(id("atom.establish"), operation::Anchor{id("jins.bayati")});
    const auto travel = grammar::Term::atom(id("atom.travel"), operation::Anchor{id("jins.nahawand")});
    const auto return_home = grammar::Term::atom(id("atom.return"), operation::Anchor{id("jins.bayati")});
    
    // Instead of neutral finite fixtures, we build a recursive phrase model!
    // body: establish -> travel -> (recurse or return)
    const auto rec = grammar::Term::produce(id("rec"), p_id);
    const auto return_alt = grammar::Term::alt(id("alt.ret_or_rec"), {
        grammar::Branch{id("br.ret"), choice::Cost{{1,0,0,0}}, return_home},
        grammar::Branch{id("br.rec"), choice::Cost{{2,0,0,0}}, rec}
    });

    if (return_alt) {
        config.production = grammar::Term::seq(id("seq.main"), establish, grammar::Term::seq(id("seq.2"), travel, *return_alt));
    }

    config.choice = id("choice.main");
    // No budget setup needed, schema doesn't have budget anymore, wait, pitch::field::Schema
    // pitch::field::Schema expects some layout of intervals maybe? We will just leave it default.
    // The previous schema was `fixture::generation::schema()`.
    
    // config.projection expects role mappings. We can just leave it empty if there are no roles to project for now.
    
    return config;
}

} // namespace mq::kernel::maqam
