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
void order();
void feasibility();
void feasibility_limits();
void witness();
void constraint();
void path();

namespace tendency {

void solve();
void laws();

} // namespace tendency

namespace sequence {

void solve();
void laws();

} // namespace sequence

namespace field {

void assembly();
void solve();

} // namespace field

namespace project {

void run();
void scope();

} // namespace project

namespace event {

void laws();
void scope();

} // namespace event

namespace request {

void run();
void motion();

} // namespace request

namespace grammar {

void laws();
void relation();

} // namespace grammar

} // namespace test
