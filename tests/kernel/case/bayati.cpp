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
    require(scaffold.has_value(), scaffold.error_or("failed to construct bayati scaffold"));
    
    // As per assessment, Bayati is currently an explicit non-generative scaffold.
    // It should not claim to be a functional generator or test non-existent stages yet.
}
