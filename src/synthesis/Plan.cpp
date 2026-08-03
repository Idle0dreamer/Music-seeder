#include "mq/synthesis/Plan.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Catalog.hpp"

#include <algorithm>
#include <sstream>
#include <optional>

namespace mq::synthesis {
namespace {

std::expected<GeneratedPlan, std::string> make_phrase(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing,
    const std::optional<::mq::kernel::Identity>& avoid) {
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
    std::optional<GeneratedPlan> selected_plan;
    constexpr std::uint64_t retry_stride = 0x9e3779b97f4a7c15ULL;
    for (std::size_t attempt = 0; attempt < 32; ++attempt) {
        const auto generated = engine.run(
            seed + static_cast<std::uint64_t>(attempt) * retry_stride,
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
        if (avoid && generated->legal.size() > 1 &&
            selected->candidate == *avoid) {
            continue;
        }
        selected_plan = GeneratedPlan{
            selected->candidate,
            selected->plan,
            {selected->candidate},
        };
        break;
    }
    if (!selected_plan) {
        return std::unexpected(
            "could not select a non-repeating legal phrase for " +
            std::string(maqam));
    }

    // Every outcome here is already a complete, legal candidate. Selection is
    // made by the kernel for this phrase seed; the player never manufactures
    // variation by cycling through route order.
    return std::move(*selected_plan);
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
    const auto first = make_phrase(maqam, seed, timing, std::nullopt);
    if (!first) {
        return std::unexpected(first.error());
    }
    GeneratedPlan result{
        first->candidate,
        first->plan,
        {first->candidate},
    };
    for (std::size_t index = 1; index < repetitions; ++index) {
        const auto continuation = make_phrase(
            maqam,
            seed + static_cast<std::uint64_t>(index) * 0x9e3779b97f4a7c15ULL,
            timing,
            result.phrase_candidates.back());
        if (!continuation) {
            return std::unexpected(continuation.error());
        }
        result.phrase_candidates.push_back(continuation->candidate);
        append_plan(result.plan, continuation->plan);
    }
    return result;
}

std::string describe_plan(
    const ::mq::kernel::performance::Plan& plan) {
    std::ostringstream output;
    output << "structure:\n";
    for (std::size_t index = 0; index < plan.events.size(); ++index) {
        const auto& event = plan.events[index];
        const auto identity = [](const auto& value) {
            return value ? value->identity.str() : std::string("none");
        };
        output << "  event[" << index << "]"
               << " onset=" << event.onset.str()
               << " duration=" << event.duration.str()
               << " structural=" << event.target.event.identity.str()
               << " cell=" << identity(event.target.cell)
               << " formula=" << identity(event.target.formula)
               << " variation=" << identity(event.target.variation)
               << " motif=" << identity(event.target.motif)
               << " transformation=" << identity(event.target.transformation)
               << " transformation_provenance="
               << (event.target.transformation_provenance.empty()
                       ? "none"
                       : event.target.transformation_provenance)
               << " direction="
               << ::mq::kernel::motion::name(event.target.event.direction)
               << " articulation="
               << static_cast<int>(event.articulation)
               << " release="
               << (event.release ? event.release->duration.str() : "none")
               << " ornament="
               << (event.ornament ? event.ornament->family.str() : "none")
               << '\n';
    }
    for (std::size_t index = 0; index < plan.pauses.size(); ++index) {
        const auto& pause = plan.pauses[index];
        output << "  pause[" << index << "]"
               << " onset=" << pause.onset.str()
               << " duration=" << pause.duration.str()
               << " function=" << pause.function.str()
               << " provenance=" << pause.provenance << '\n';
    }
    return output.str();
}

} // namespace mq::synthesis
