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
void trajectory();
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

namespace timed {

void plan();

} // namespace timed

namespace request {

void run();
void motion();

} // namespace request

namespace phrase {

void laws();
void scope();
void projection();

} // namespace phrase

namespace generate {

void laws();
void profile();
void grammar();
void overlay();

} // namespace generate

namespace jins {

void catalog();
void overlay();

} // namespace jins

namespace gesture {

void laws();
void scope();

} // namespace gesture

namespace sayr {

void plan();
void laws();
void scope();

} // namespace sayr

namespace grammar {

void laws();
void relation();
void produce();

} // namespace grammar

void bayati_case();

} // namespace test
