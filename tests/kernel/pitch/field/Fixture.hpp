#pragma once

#include "mq/kernel/pitch/field/Schema.hpp"

namespace test::field {

struct Fixture {
    mq::kernel::Identity root;
    mq::kernel::Identity upper;
    mq::kernel::Identity direction;
    mq::kernel::Identity up;
    mq::kernel::Identity down;
    mq::kernel::Identity strong;
    mq::kernel::Identity weak;
    mq::kernel::Identity anchor;
    mq::kernel::Identity rising;
    mq::kernel::Identity falling;
    mq::kernel::Identity surface;
    mq::kernel::pitch::field::Schema schema;
};

[[nodiscard]] Fixture fixture();

} // namespace test::field
