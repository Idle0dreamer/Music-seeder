#include "../Test.hpp"

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"
#include "mq/kernel/grammar/Catalog.hpp"
#include "mq/kernel/grammar/Evaluator.hpp"
#include "mq/kernel/jins/Catalog.hpp"
#include "mq/kernel/path/Graph.hpp"
#include "mq/kernel/profile/Set.hpp"
#include "mq/kernel/maqam/Bayati.hpp"

namespace {

using namespace mq::kernel;

} // namespace

void test::bayati_case() {
    using namespace mq::kernel;
    auto scaffold = maqam::make_bayati();
    require(!scaffold.has_value(), "Bayati scaffold should fail without sourced sayr route");
    require(scaffold.error() == "missing sourced sayr route for Bayati", "expected incomplete sayr error");
}
