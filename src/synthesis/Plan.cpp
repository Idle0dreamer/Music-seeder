#include "mq/synthesis/Plan.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>

namespace mq::synthesis {
namespace {

std::expected<GeneratedPlan, std::string> make_phrase(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    std::size_t continuation) {
    const auto catalog = ::mq::kernel::maqam::Catalog::declared();
    const auto scaffold = catalog.build_executable(maqam);
    if (!scaffold) {
        return std::unexpected(scaffold.error());
    }
    const ::mq::kernel::eval::Context context{
        .jins = {&scaffold->ajnas},
        .path = {&scaffold->graph},
        .sayr = {&scaffold->sayr},
        .grammar = {},
    };
    const ::mq::kernel::generate::Engine engine(*scaffold->profile, context);
    const auto generated = engine.run(
        seed,
        scaffold->generation.choice,
        scaffold->generation.production,
        scaffold->generation.projection,
        scaffold->generation.schema,
        {},
        ::mq::kernel::generate::Limits{.timing = timing});
    if (!generated) {
        return std::unexpected(generated.error().message);
    }

    const auto selected = std::ranges::find(
        generated->legal,
        generated->selected,
        &::mq::kernel::generate::Outcome::candidate);
    if (selected == generated->legal.end()) {
        return std::unexpected(
            "selected maqam outcome is missing: " + std::string(maqam));
    }

    // Every outcome here is already a complete, legal candidate. The first
    // phrase follows the engine's cost-aware selection. Continuations rotate
    // through that legal set so a multi-route collection package can develop
    // instead of replaying its first route forever.
    const auto selected_index = static_cast<std::size_t>(
        std::distance(generated->legal.begin(), selected));
    const auto index = continuation == 0 || generated->legal.size() == 1
                           ? selected_index
                           : (selected_index + continuation) %
                                 generated->legal.size();
    const auto& outcome = generated->legal[index];
    return GeneratedPlan{outcome.candidate, outcome.plan};
}

} // namespace

void append_plan(
    ::mq::kernel::performance::Plan& destination,
    const ::mq::kernel::performance::Plan& continuation) {
    const auto offset = destination.end();
    for (const auto& event : continuation.events) {
        auto copied = event;
        copied.onset += offset;
        destination.events.push_back(std::move(copied));
    }
    for (const auto& pause : continuation.pauses) {
        auto copied = pause;
        copied.onset += offset;
        destination.pauses.push_back(std::move(copied));
    }
}

std::expected<GeneratedPlan, std::string> make_plan(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    std::size_t repetitions) {
    if (repetitions == 0) {
        return std::unexpected("at least one performance phrase is required");
    }
    const auto first = make_phrase(maqam, seed, timing, 0);
    if (!first) {
        return std::unexpected(first.error());
    }
    GeneratedPlan result = *first;
    for (std::size_t index = 1; index < repetitions; ++index) {
        const auto continuation = make_phrase(
            maqam,
            seed + static_cast<std::uint64_t>(index) * 0x9e3779b97f4a7c15ULL,
            timing,
            index);
        if (!continuation) {
            return std::unexpected(continuation.error());
        }
        append_plan(result.plan, continuation->plan);
    }
    return result;
}

} // namespace mq::synthesis
