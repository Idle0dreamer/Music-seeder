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
        test::path();
        test::grammar::laws();
        test::grammar::relation();
        std::cout
            << "PASS exact rational arithmetic\n"
            << "PASS symbolic calculated pitch fields\n"
            << "PASS regional reconstruction laws\n"
            << "PASS evidence-gated tonal operators\n"
            << "PASS stable seeded choice laws\n"
            << "PASS path history and prerequisite guards\n"
            << "PASS grammar identity, sequence, repeat, and scope laws\n"
            << "PASS complete grammar branch evaluation\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL " << error.what() << '\n';
        return 1;
    }
}
