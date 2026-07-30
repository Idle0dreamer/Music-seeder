#include "Test.hpp"

#include <exception>
#include <iostream>

int main() {
    try {
        rationalTests();
        pitchTests();
        profileTests();
        evaluatorTests();
        choiceTests();
        std::cout
            << "PASS exact rational arithmetic\n"
            << "PASS symbolic calculated pitch fields\n"
            << "PASS regional reconstruction laws\n"
            << "PASS evidence-gated tonal operators\n"
            << "PASS stable seeded choice laws\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL " << error.what() << '\n';
        return 1;
    }
}

