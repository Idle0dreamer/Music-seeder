#include "Run.hpp"

#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/grammar/Catalog.hpp"
#include "mq/kernel/maqam/Bayati.hpp"

#include <algorithm>
#include <iostream>

namespace app::generate {
namespace {

const char* ornament_name(
    mq::kernel::performance::OrnamentKind kind) noexcept {
    switch (kind) {
    case mq::kernel::performance::OrnamentKind::Approach:
        return "approach";
    case mq::kernel::performance::OrnamentKind::Oscillation:
        return "oscillation";
    }
    return "unknown";
}

void print_timed(const mq::kernel::performance::TimedEvent& timed) {
    const auto& target = timed.target;
    std::cout
        << "  " << target.event.identity.str()
        << " -> " << target.center.str()
        << " @ " << timed.onset.str()
        << " + " << timed.duration.str()
        << " intensity " << timed.intensity.str();
    if (timed.contour) {
        std::cout << " contour "
                  << timed.contour->points.front().offset.str()
                  << " -> " << timed.contour->points.back().offset.str();
    }
    if (timed.ornament) {
        std::cout << " ornament " << ornament_name(timed.ornament->kind)
                  << " extent " << timed.ornament->extent.str();
    }
    std::cout << '\n';
}

} // namespace

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
        << "timed performance events: " << selected->plan.events.size()
        << '\n';
    for (const auto& timed : selected->plan.events) {
        print_timed(timed);
    }
    return 0;
}

int bayati(std::uint64_t seed) {
    using namespace mq::kernel;

    const auto scaffold = maqam::make_bayati();
    if (!scaffold) {
        std::cerr << scaffold.error() << '\n';
        return 1;
    }
    const eval::Context context{
        .jins = {&scaffold->ajnas},
        .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr},
        .grammar = {nullptr},
    };
    const mq::kernel::generate::Engine engine(*scaffold->profile, context);
    const state::Snapshot initial;
    const auto result = engine.run(
        seed,
        scaffold->generation.choice,
        scaffold->generation.production,
        scaffold->generation.projection,
        scaffold->generation.schema,
        initial);
    if (!result) {
        std::cerr << result.error().message << '\n';
        for (const auto& rejection : result.error().rejected) {
            std::cerr << "  rejected " << rejection.candidate.str()
                      << ": " << rejection.message << '\n';
        }
        return 1;
    }
    const auto selected = std::ranges::find(
        result->legal,
        result->selected,
        &mq::kernel::generate::Outcome::candidate);
    if (selected == result->legal.end()) {
        std::cerr << "selected Bayati outcome is missing\n";
        return 1;
    }
    std::cout
        << "profile: " << scaffold->profile->identity() << '\n'
        << "seed: " << seed << '\n'
        << "candidate: " << result->selected.str() << '\n'
        << "legal Bayati routes: " << result->legal.size() << '\n'
        << "rejected Bayati routes: " << result->rejected.size() << '\n'
        << "timed performance events: " << selected->plan.events.size()
        << '\n';
    for (const auto& rejection : result->rejected) {
        std::cout << "  rejected " << rejection.candidate.str()
                  << ": " << rejection.message << '\n';
    }
    for (const auto& timed : selected->plan.events) {
        print_timed(timed);
    }
    return 0;
}

} // namespace app::generate
