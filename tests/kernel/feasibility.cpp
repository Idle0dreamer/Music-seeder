#include "Test.hpp"

#include "mq/kernel/pitch/System.hpp"

#include <utility>

void test::feasibility() {
    using namespace mq::kernel;
    namespace pf = pitch::feasibility;

    const Identity x{"test.feasibility", "x", "1"};
    const Identity y{"test.feasibility", "y", "1"};
    const auto identity = [](std::string name) {
        return Identity{
            "test.feasibility.constraint",
            std::move(name),
            "1",
        };
    };

    pitch::System open;
    open.declare(x);
    open.declare(y);
    open.bound({
        identity("open"),
        {{x, Rational(1)}, {y, Rational(-1)}},
        {},
        "x at or below y",
    });
    const auto openReport = open.feasible();
    require(
        openReport &&
            openReport->status == pf::Status::Feasible &&
            openReport->solution,
        "underdetermined pitch inequality was not recognized as feasible");

    pitch::System chain;
    chain.declare(x);
    chain.declare(y);
    chain.bound({
        identity("chain.xy"),
        {{x, Rational(1)}, {y, Rational(-1)}},
        {},
        "x at or below y",
    });
    chain.bound({
        identity("chain.y"),
        {{y, Rational(1)}},
        {},
        "y at or below zero",
    });
    chain.bound({
        identity("chain.x"),
        {{x, Rational(-1)}},
        pitch::Expression::cents(Rational(-1)),
        "x at or above one",
    });
    const auto chainReport = chain.feasible();
    require(
        chainReport && chainReport->status == pf::Status::Infeasible,
        "cross-variable inequality contradiction was missed");
    require(
        chainReport->proof &&
            chainReport->proof->relation == pitch::order::Relation::Greater &&
            chainReport->constraints.size() == 3 &&
            chainReport->provenance.size() == 3,
        "inequality contradiction lost its proof or provenance");

    const auto fourth = pitch::Expression::ratio(4, 3);
    pitch::System symbolic;
    symbolic.declare(x);
    symbolic.equate({
        identity("symbolic.equal"),
        {{x, Rational(1)}},
        fourth,
        "exact fourth",
    });
    symbolic.bound({
        identity("symbolic.bound"),
        {{x, Rational(1)}},
        pitch::Expression::cents(Rational(498)),
        "498 cent ceiling",
    });
    const auto symbolicReport = symbolic.feasible();
    require(
        symbolicReport && symbolicReport->status == pf::Status::Infeasible,
        "symbolic ratio inequality was rounded into feasibility");

    pitch::System boundary;
    boundary.declare(x);
    boundary.equate({
        identity("boundary.equal"),
        {{x, Rational(1)}},
        fourth,
        "exact fourth",
    });
    boundary.bound({
        identity("boundary.bound"),
        {{x, Rational(1)}},
        fourth,
        "exact ceiling",
    });
    const auto boundaryReport = boundary.feasible();
    require(
        boundaryReport &&
            boundaryReport->status == pf::Status::Feasible &&
            boundaryReport->solution &&
            boundaryReport->solution->values.at(x) == fourth,
        "closed exact inequality rejected its equality boundary");

    pitch::System unsafe;
    unsafe.declare(x);
    unsafe.equate({
        identity("unsafe.equal"),
        {{x, Rational(1)}},
        {},
        "zero anchor",
    });
    unsafe.bound({
        identity("unsafe.bound"),
        {{x, Rational(-1)}},
        pitch::Expression::cents(Rational(-1)),
        "one cent floor",
    });
    require(
        !unsafe.solve(),
        "equality solver ignored a contradictory inequality");

}
