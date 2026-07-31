#include "../Test.hpp"

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/grammar/Catalog.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"
#include "mq/kernel/jins/Catalog.hpp"
#include "mq/kernel/path/Graph.hpp"
#include "mq/kernel/profile/Set.hpp"
#include "mq/kernel/sayr/Plan.hpp"

namespace {

using namespace mq::kernel;

Identity id(const std::string& name) {
    return {"bayati", name, "1"};
}

// Reconstruct a Bayati profile per theory/case/bayati.md
profile::Set build_bayati_profile() {
    profile::Set base("bayati.base");
    
    // According to bayati.md:
    // "No interval array is accepted as a substitute."
    // We must supply:
    // - tonic role and ghammaz candidates;
    // - characteristic register and register-dependent variants;
    // - emphasis and attraction roles;
    // - baggage tones outside the narrow jins span;
    // - entry, establishment, departure, and return cells;
    // - cadence families and closure strengths;
    // - contextual intonation trajectories;
    // - performer and repertoire variants.

    // Intonation parameters (not a cents array)
    // Bayati tonic
    base = *profile::reconstruct(
        "bayati.reconstruct",
        {base},
        {
            {
                profile::Patch::Action::Define,
                "bayati.role.tonic",
                profile::Rule{
                    profile::Ordering{id("role.d")},
                    {"bayati.md"}
                }
            },
            {
                profile::Patch::Action::Define,
                "bayati.role.ghammaz",
                profile::Rule{
                    profile::Ordering{id("role.g")},
                    {"bayati.md"}
                }
            },
            // ... more to come
        }
    );
    return base;
}

jins::Catalog build_bayati_ajnas() {
    jins::Catalog catalog;
    // Jins Bayati on D
    // Needs tonic, ghammaz, register, gestures
    jins::Descriptor bayati{
        id("jins.bayati"),
        id("role.d"), // tonic
        {id("role.d"), id("role.e_half_flat"), id("role.f"), id("role.g")}, // span
        {id("role.g")}, // ghammaz
        {id("region.middle")},
        {id("region.middle")},
        {id("role.d"), id("role.g")}, // emphasis
        {}, // entry
        {}, // exit
        {}, // cadences
        {}, // motifs
        {}, // baggage
        {}  // gestures
    };
    auto result = catalog.add(bayati);
    if (!result) throw std::runtime_error("failed to add bayati: " + result.error());
    return catalog;
}

path::Graph build_bayati_graph() {
    path::Graph graph;
    // According to bayati.md:
    // network involving Bayati, Rast, Nahawand, Hijaz, and Ajam at particular tonal roles.
    auto result = graph.add(path::Rule{
        id("path.bayati_to_rast"),
        id("jins.bayati"),
        id("jins.rast"),
        tonicization::Level::Internal,
        {},
        "bayati.md"
    });
    if (!result) throw std::runtime_error("failed to add path: " + result.error());
    return graph;
}

sayr::Plan build_bayati_sayr() {
    sayr::Plan plan;
    // Must implement Minimum phrase tests:
    // 1. Establish root Bayati without a foreign-center claim.
    // 2. Introduce a brief color and return without mislabeling it as modulation.
    // ...
    return plan;
}

} // namespace

void test::bayati_case() {
    using namespace mq::kernel;
    
    const auto profile = build_bayati_profile();
    const auto ajnas = build_bayati_ajnas();
    const auto graph = build_bayati_graph();
    const auto plan = build_bayati_sayr();

    mq::kernel::grammar::Catalog catalog;
    [[maybe_unused]] mq::kernel::grammar::Evaluator eval(
        profile,
        ajnas,
        graph,
        catalog
    );

    // Test 1: Establish root Bayati
    // Test 2: Brief color
    // Test 3: Internal jins
    // ...
    require(true, "Bayati case placeholder");
}
