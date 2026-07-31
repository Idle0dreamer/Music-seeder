#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/grammar/Catalog.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"

namespace {

namespace kg = mq::kernel::grammar;

mq::kernel::Identity id(const std::string& name) {
    return {"test.grammar", name, "1"};
}

} // namespace

void test::grammar::produce() {
    using namespace mq::kernel;
    
    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    
    kg::Catalog catalog;
    
    // A grammar production that expands to: 
    // anchor then emphasize(1) then recurse(itself)
    const auto p_id = id("recursive_prod");
    
    const auto anchor = kg::Term::atom(
        id("anchor"),
        operation::Anchor{fixture.center.root});
    const auto emphasis = kg::Term::atom(
        id("emphasis"),
        operation::Emphasize{fixture.role.root, Rational(1)});
    const auto rec = kg::Term::produce(id("rec"), p_id);
    
    const auto body = kg::Term::alt(
        id("alt"),
        {
            kg::Branch{id("br.term"), choice::Cost{{1,0,0,0}}, anchor},
            kg::Branch{id("br.rec"), choice::Cost{{0,0,0,0}}, kg::Term::seq(id("seq2"), emphasis, rec)}
        });
    require(body.has_value(), "failed to create body alt");
        
    require(catalog.add(p_id, *body).has_value(), "failed to add production");
    
    const kg::Evaluator grammar_eval(
        fixture.profile.shared,
        fixture.catalog,
        fixture.path.graph,
        catalog);

    state::Snapshot initial;
    initial.jins.active = fixture.jins.root;
    // Set budget to 2 for this production
    initial.grammar.budget[p_id] = 2;
    
    const auto result = grammar_eval.derive(rec, initial);
    
    // It should explore recursion up to budget 2, producing terminating paths at depths 0, 1, 2
    require(!result.outcomes.empty(), "recursive production did not yield successful outcomes");
    require(result.outcomes.size() >= 2, "expected multiple valid depths of recursion outcomes");
}
