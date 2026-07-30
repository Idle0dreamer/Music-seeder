#pragma once

#include "mq/kernel/sayr/need/Center.hpp"
#include "mq/kernel/sayr/need/Evidence.hpp"
#include "mq/kernel/sayr/need/Gesture.hpp"
#include "mq/kernel/sayr/need/Jins.hpp"
#include "mq/kernel/sayr/need/Path.hpp"
#include "mq/kernel/sayr/need/Phrase.hpp"
#include "mq/kernel/sayr/need/Region.hpp"
#include "mq/kernel/sayr/need/Role.hpp"

#include <variant>

namespace mq::kernel::sayr {

using Requirement = std::variant<
    need::Jins,
    need::Center,
    need::Role,
    need::Region,
    need::Gesture,
    need::Phrase,
    need::Path,
    need::Evidence>;

struct Need {
    Identity identity;
    Requirement requirement;
};

} // namespace mq::kernel::sayr
