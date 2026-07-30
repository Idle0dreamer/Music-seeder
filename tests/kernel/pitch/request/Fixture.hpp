#pragma once

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/pitch/field/project/Plan.hpp"
#include "mq/kernel/pitch/field/Schema.hpp"

namespace test::request {

struct Case {
    mq::kernel::Identity first;
    mq::kernel::Identity second;
    mq::kernel::Identity third;
    mq::kernel::Identity fourth;
    mq::kernel::Identity rise;
    mq::kernel::pitch::field::project::Plan projection;
    mq::kernel::pitch::field::Schema schema;
};

[[nodiscard]] Case make(
    const mq::kernel::fixture::Set& fixture);

} // namespace test::request
