#include "mq/synthesis/Plan.hpp"

#include "mq/kernel/generate/Engine.hpp"
#include "mq/kernel/maqam/Ajam.hpp"
#include "mq/kernel/maqam/Bayati.hpp"
#include "mq/kernel/maqam/Hijaz.hpp"
#include "mq/kernel/maqam/Kurd.hpp"
#include "mq/kernel/maqam/Nahawand.hpp"
#include "mq/kernel/maqam/Nikriz.hpp"
#include "mq/kernel/maqam/Rast.hpp"
#include "mq/kernel/maqam/Sikah.hpp"

#include <algorithm>
#include <array>

namespace mq::synthesis {
namespace {

using Builder = std::expected<
    ::mq::kernel::maqam::Scaffold,
    std::string> (*)();

struct Route {
    std::string_view name;
    Builder builder;
};

constexpr std::array routes{
    Route{"ajam", ::mq::kernel::maqam::make_ajam},
    Route{"bayati", ::mq::kernel::maqam::make_bayati},
    Route{"hijaz", ::mq::kernel::maqam::make_hijaz},
    Route{"kurd", ::mq::kernel::maqam::make_kurd},
    Route{"nahawand", ::mq::kernel::maqam::make_nahawand},
    Route{"nikriz", ::mq::kernel::maqam::make_nikriz},
    Route{"rast", ::mq::kernel::maqam::make_rast},
    Route{"sikah", ::mq::kernel::maqam::make_sikah},
};

const Route* find_route(std::string_view name) noexcept {
    const auto found = std::ranges::find(routes, name, &Route::name);
    return found == routes.end() ? nullptr : &*found;
}

} // namespace

std::expected<GeneratedPlan, std::string> make_plan(
    std::string_view maqam,
    std::uint64_t seed,
    const ::mq::kernel::performance::Timing& timing) {
    const auto* route = find_route(maqam);
    if (route == nullptr) {
        return std::unexpected("unknown maqam route: " + std::string(maqam));
    }
    const auto scaffold = route->builder();
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
    return GeneratedPlan{generated->selected, selected->plan};
}

} // namespace mq::synthesis
