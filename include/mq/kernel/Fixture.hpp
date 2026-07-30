#pragma once

#include "mq/kernel/Profile.hpp"

#include <expected>
#include <string>

namespace mq::kernel {

struct Fixture {
    Identity rootCenter;
    Identity branchCenter;
    Identity rootJins;
    Identity branchJins;
    Identity rootRole;
    Identity ghammazRole;
    Identity cell;
    Identity cadence;
    Identity path;
    Profile shared;
    Profile regionalA;
    Profile regionalB;
};

[[nodiscard]] std::expected<Fixture, std::string> makeFixture();

} // namespace mq::kernel

