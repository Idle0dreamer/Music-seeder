#include "Run.hpp"

#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/grammar/Catalog.hpp"

#include <algorithm>
#include <iostream>

namespace app::generate {

int run(std::uint64_t seed) {
    using namespace mq::kernel;

    const auto set = fixture::make();
    if (!set) {
        std::cerr << set.error() << '\n';
        return 1;
    }
    const auto model = fixture::generation::make(*set);
    if (!model) {
        std::cerr << model.error() << '\n';
        return 1;
    }
    
    // The CLI currently runs the neutral fixture's finite grammar
    // without recursive productions.
    
    const eval::Context context{
        .jins = {&set->catalog},
        .path = {&set->path.graph},
        .sayr = {&set->sayr.plan},
        .grammar = {nullptr},
    };
    const mq::kernel::generate::Engine engine(
        set->profile.shared,
        context);
    
    // Specify initial snapshot and budget
    state::Snapshot initial;

    const auto result = engine.run(
        seed,
        model->choice,
        model->production,
        model->projection,
        model->schema,
        initial);
    if (!result) {
        std::cerr << result.error().message << '\n';
        return 1;
    }
    const auto selected = std::ranges::find(
        result->legal,
        result->selected,
        &mq::kernel::generate::Outcome::candidate);
    if (selected == result->legal.end()) {
        std::cerr << "selected outcome is missing\n";
        return 1;
    }

    std::cout
        << "seed: " << seed << '\n'
        << "candidate: " << result->selected.str() << '\n'
        << "legal candidates: " << result->legal.size() << '\n'
        << "rejected grammar branches: "
        << result->derivation.size() << '\n'
        << "structural targets: " << selected->plan.targets.size() << '\n';
    for (const auto& target : selected->plan.targets) {
        std::cout
            << "  " << target.event.identity.str()
            << " -> " << target.center.str() << '\n';
    }
    return 0;
}

} // namespace app::generate
