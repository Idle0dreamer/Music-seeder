#pragma once

#include "mq/kernel/performance/Plan.hpp"
#include "mq/synthesis/Render.hpp"

#include <expected>
#include <string>

namespace mq::synthesis {

[[nodiscard]] std::expected<RenderReport, RenderError> render_faust_wav(
    const ::mq::kernel::performance::Plan& plan,
    const RenderConfig& config,
    const std::string& path);

} // namespace mq::synthesis
