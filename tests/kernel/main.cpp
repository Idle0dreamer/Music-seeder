#include "Test.hpp"

#include <exception>
#include <iostream>

int main() {
    try {
        test::rational();
        test::pitch();
        test::profile();
        test::evaluator();
        test::choice();
        test::order();
        test::feasibility();
        test::feasibility_limits();
        test::witness();
        test::constraint();
        test::trajectory();
        test::tendency::solve();
        test::tendency::laws();
        test::sequence::solve();
        test::sequence::laws();
        test::field::assembly();
        test::field::solve();
        test::project::run();
        test::project::scope();
        test::jins::catalog();
        test::jins::overlay();
        test::event::laws();
        test::event::scope();
        test::timed::plan();
        test::gesture::laws();
        test::gesture::scope();
        test::sayr::plan();
        test::sayr::laws();
        test::sayr::scope();
        test::request::run();
        test::request::motion();
        test::phrase::laws();
        test::phrase::scope();
        test::phrase::projection();
        test::generate::laws();
        test::generate::profile();
        test::generate::grammar();
        test::generate::overlay();
        test::path();
        test::grammar::laws();
        test::grammar::relation();
        test::grammar::produce();
        test::maqam_catalog();
        test::ajam_case();
        test::hijaz_case();
        test::kurd_case();
        test::nahawand_case();
        test::nikriz_case();
        test::rast_case();
        test::bayati_case();
        std::cout
            << "PASS exact rational arithmetic\n"
            << "PASS symbolic calculated pitch fields\n"
            << "PASS regional reconstruction laws\n"
            << "PASS evidence-gated tonal operators\n"
            << "PASS stable seeded choice laws\n"
            << "PASS proof-producing symbolic pitch order\n"
            << "PASS exact affine inequality feasibility\n"
            << "PASS exact hard-constraint witness construction\n"
            << "PASS stable hard-constraint identity laws\n"
            << "PASS continuous intonation trajectory basis functions\n"
            << "PASS exact contextual tendency KKT solving\n"
            << "PASS tendency identity, budget, and ordering laws\n"
            << "PASS exact lexicographic tendency sequencing\n"
            << "PASS tendency non-degradation and tier-order laws\n"
            << "PASS contextual pitch-field assembly laws\n"
            << "PASS calculated field-to-ordering integration\n"
            << "PASS typed state-to-field projection laws\n"
            << "PASS scoped grammar-to-field integration\n"
            << "PASS validated jins descriptor catalog laws\n"
            << "PASS provenance-bearing descriptor overlay operations\n"
            << "PASS structural event transition laws\n"
            << "PASS structural event scope laws\n"
            << "PASS exact timed contour and ornament laws\n"
            << "PASS ordered jins gesture and baggage laws\n"
            << "PASS gesture scope and dependency laws\n"
            << "PASS validated sayr plan and route laws\n"
            << "PASS checked sayr fulfillment and proof laws\n"
            << "PASS sayr scope restoration laws\n"
            << "PASS exact instrument-neutral pitch request laws\n"
            << "PASS exact same and fall motion laws\n"
            << "PASS phrase span and cadence transition laws\n"
            << "PASS phrase scope and dependency laws\n"
            << "PASS phrase-conditioned pitch request laws\n"
            << "PASS staged complete-candidate generation laws\n"
            << "PASS executable profile-to-generation laws\n"
            << "PASS typed grammar-to-generation laws\n"
            << "PASS path history and prerequisite guards\n"
            << "PASS grammar identity, sequence, repeat, and scope laws\n"
            << "PASS well-founded recursive phrase production budgets\n"
            << "PASS declared maqam catalog refuses incomplete packages\n"
            << "PASS executable Ajam routes and timed plans\n"
            << "PASS executable Hijaz routes and timed plans\n"
            << "PASS executable Kurd routes and timed plans\n"
            << "PASS executable Nahawand routes and timed plans\n"
            << "PASS executable Nikriz routes and timed plans\n"
            << "PASS executable Rast routes and timed plans\n"
            << "PASS executable provisional Bayati routes and timed plans\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL " << error.what() << '\n';
        return 1;
    }
}
