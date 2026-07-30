#pragma once

#include "mq/kernel/jins/Catalog.hpp"
#include "mq/kernel/path/Graph.hpp"
#include "mq/kernel/sayr/Plan.hpp"

namespace mq::kernel::eval {

struct Context {
    struct Jins {
        const mq::kernel::jins::Catalog* catalog{};
    } jins;

    struct Path {
        const mq::kernel::path::Graph* graph{};
    } path;

    struct Sayr {
        const mq::kernel::sayr::Plan* plan{};
    } sayr;
};

} // namespace mq::kernel::eval
