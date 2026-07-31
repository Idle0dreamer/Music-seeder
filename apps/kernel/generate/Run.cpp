#include "Run.hpp"

#include "mq/kernel/maqam/Bayati.hpp"
#include "mq/kernel/generate/Engine.hpp"

#include <algorithm>
#include <iostream>

namespace app::generate {

int run(std::uint64_t seed) {
    using namespace mq::kernel;

    const auto model = maqam::make_bayati();
    if (!model) {
        std::cerr << model.error() << '\n';
        return 1;
    }
    const eval::Context context{
        .jins = {&model->ajnas},
        .path = {&model->graph},
        .sayr = {&model->sayr},
        .grammar = {},
    };
    const mq::kernel::generate::Engine engine(
        *model->profile,
        context);
    
    if (!model->production) {
        std::cerr << "missing recursive production rule\n";
        return 1;
    }

    const auto result = engine.run(
        seed,
        model->choice,
        *model->production,
        model->projection,
        model->schema);
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
