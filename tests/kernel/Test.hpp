#pragma once

#include <stdexcept>
#include <string>

inline void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

namespace test {

void rational();
void pitch();
void profile();
void evaluator();
void choice();
void path();

namespace grammar {

void laws();
void relation();

} // namespace grammar

} // namespace test
