#include "Run.hpp"

#include "mq/kernel/fixture/generation/Set.hpp"
#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/grammar/Catalog.hpp"
#include "mq/kernel/maqam/Catalog.hpp"
#include "mq/kernel/performance/Profile.hpp"

#include <algorithm>
#include <iostream>
#include <string_view>

namespace app::generate {
namespace {

constexpr std::string_view timing_path =
    "theory/data/performance/free-rhythm-v1.timing";

std::expected<mq::kernel::performance::Timing, std::string> timing_profile() {
    return mq::kernel::performance::load_timing_profile(timing_path);
}

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
    switch (timed.articulation) {
    case mq::kernel::performance::Articulation::Neutral:
        std::cout << " articulation neutral";
        break;
    case mq::kernel::performance::Articulation::Connected:
        std::cout << " articulation connected";
        break;
    case mq::kernel::performance::Articulation::Detached:
        std::cout << " articulation detached";
        break;
    }
    if (timed.contour) {
        std::cout << " contour "
                  << timed.contour->points.front().offset.str()
                  << " -> " << timed.contour->points.back().offset.str();
    }
    if (timed.ornament) {
        std::cout << " ornament " << ornament_name(timed.ornament->kind)
                  << " family " << timed.ornament->family.str()
                  << " extent " << timed.ornament->extent.str()
                  << " timing " << timed.ornament->timing.onset.str()
                  << "/" << timed.ornament->timing.duration.str()
                  << "/" << timed.ornament->timing.release.str();
    }
    if (timed.release) {
        std::cout << " release + " << timed.release->duration.str();
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
    const auto timing = timing_profile();
    if (!timing) {
        std::cerr << timing.error() << '\n';
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
        initial,
        mq::kernel::generate::Limits{.timing = *timing});
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
    for (const auto& pause : selected->plan.pauses) {
        std::cout << "  pause " << pause.function.str() << " @ "
                  << pause.onset.str() << " + " << pause.duration.str()
                  << '\n';
    }
    return 0;
}

int maqam(
    std::uint64_t seed,
    std::expected<mq::kernel::maqam::Scaffold, std::string> scaffold,
    std::string_view name) {
    using namespace mq::kernel;

    if (!scaffold) {
        std::cerr << scaffold.error() << '\n';
        return 1;
    }
    const auto timing = timing_profile();
    if (!timing) {
        std::cerr << timing.error() << '\n';
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
        initial,
        mq::kernel::generate::Limits{
            .timing = *timing,
        });
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
        std::cerr << "selected " << name << " outcome is missing\n";
        return 1;
    }
    std::cout
        << "profile: " << scaffold->profile->identity() << '\n'
        << "seed: " << seed << '\n'
        << "candidate: " << result->selected.str() << '\n'
        << "legal " << name << " routes: " << result->legal.size() << '\n'
        << "rejected " << name << " routes: " << result->rejected.size() << '\n'
        << "timed performance events: " << selected->plan.events.size()
        << '\n';
    for (const auto& rejection : result->rejected) {
        std::cout << "  rejected " << rejection.candidate.str()
                  << ": " << rejection.message << '\n';
    }
    for (const auto& timed : selected->plan.events) {
        print_timed(timed);
    }
    for (const auto& pause : selected->plan.pauses) {
        std::cout << "  pause " << pause.function.str() << " @ "
                  << pause.onset.str() << " + " << pause.duration.str()
                  << '\n';
    }
    return 0;
}

bool is_named(std::string_view name) noexcept {
    return mq::kernel::maqam::Catalog::declared().find(name) != nullptr;
}

int named(std::string_view name, std::uint64_t seed) {
    const auto catalog = mq::kernel::maqam::Catalog::declared();
    if (catalog.find(name) == nullptr) {
        std::cerr << "unknown maqam route: " << name << '\n';
        return 2;
    }
    return maqam(seed, catalog.build_executable(name), name);
}

} // namespace app::generate
