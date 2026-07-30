#pragma once

#include <stdexcept>
#include <string>

inline void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void rationalTests();
void pitchTests();
void profileTests();
void evaluatorTests();
void choiceTests();

