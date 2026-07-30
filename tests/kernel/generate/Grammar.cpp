#include "../Test.hpp"

#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/grammar/Term.hpp"

namespace {

namespace kg = mq::kernel::grammar;

mq::kernel::Identity id(std::string name) {
    return {"test.generate.grammar", std::move(name), "1"};
}

} // namespace

void test::generate::grammar() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& set = *made;
    const auto generated = fixture::generation::make(set);
    require(generated.has_value(), generated.error_or("generation failed"));
    const auto& value = *generated;
    const eval::Context context{
        .jins = {&set.catalog},
        .path = {&set.path.graph},
        .sayr = {&set.sayr.plan},
    };
    const struct {
        mq::kernel::generate::Engine a;
        mq::kernel::generate::Engine b;
    } regional{
        {set.profile.regional.a, context},
        {set.profile.regional.b, context},
    };

    std::optional<std::uint64_t> seed;
    for (std::uint64_t item = 0; item < 512; ++item) {
        const auto result = regional.a.run(
            item,
            value.choice,
            value.production,
            value.projection,
            value.schema);
        if (result &&
            result->selected == value.program.travel.identity) {
            seed = item;
            break;
        }
    }
    require(seed.has_value(), "grammar never selected travel");

    const auto a = regional.a.run(
        *seed,
        value.choice,
        value.production,
        value.projection,
        value.schema);
    const auto b = regional.b.run(
        *seed,
        value.choice,
        value.production,
        value.projection,
        value.schema);
    require(
        a &&
            a->selected == value.program.travel.identity &&
            a->legal.size() == 2 &&
            a->derivation.empty(),
        "typed grammar lost a legal generated candidate");
    require(
        b &&
            b->selected == value.program.stay.identity &&
            b->legal.size() == 1 &&
            b->derivation.size() == 1 &&
            b->derivation.front().violation &&
            b->derivation.front().violation->rule == "allow.baggage",
        "grammar diagnostics lost regional baggage narrowing");

    const auto scoped = kg::Term::scope(
        id("scoped.scope"),
        {id("scoped.policy"), kg::scope::Part::Output},
        value.production);
    const auto emitted = regional.a.run(
        0,
        value.choice,
        scoped,
        value.projection,
        value.schema);
    require(
        emitted && emitted->legal.size() == 2,
        "explicit Output scope did not emit its event stage");

    const auto hidden = kg::Term::scope(
        id("hidden.scope"),
        {id("hidden.policy"), kg::scope::Part::None},
        value.production);
    const auto omitted = regional.a.run(
        0,
        value.choice,
        hidden,
        value.projection,
        value.schema);
    require(
        !omitted &&
            omitted.error().code ==
                mq::kernel::generate::Error::Code::Input,
        "non-exported grammar output leaked into generation");
}
