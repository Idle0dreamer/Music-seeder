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
        test::tendency::solve();
        test::tendency::laws();
        test::sequence::solve();
        test::sequence::laws();
        test::field::assembly();
        test::field::solve();
        test::path();
        test::grammar::laws();
        test::grammar::relation();
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
            << "PASS exact contextual tendency KKT solving\n"
            << "PASS tendency identity, budget, and ordering laws\n"
            << "PASS exact lexicographic tendency sequencing\n"
            << "PASS tendency non-degradation and tier-order laws\n"
            << "PASS contextual pitch-field assembly laws\n"
            << "PASS calculated field-to-ordering integration\n"
            << "PASS path history and prerequisite guards\n"
            << "PASS grammar identity, sequence, repeat, and scope laws\n"
            << "PASS complete grammar branch evaluation\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL " << error.what() << '\n';
        return 1;
    }
}
