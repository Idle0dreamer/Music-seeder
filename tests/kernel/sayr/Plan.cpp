#include "../Test.hpp"

#include "mq/kernel/fixture/Set.hpp"
#include "mq/kernel/sayr/Plan.hpp"

#include <algorithm>

namespace ks = mq::kernel::sayr;

namespace {

mq::kernel::Identity id(std::string name) {
    return {"test.sayr.plan", std::move(name), "1"};
}

ks::Need need(
    const mq::kernel::Identity& identity,
    ks::Requirement requirement) {
    return {identity, std::move(requirement)};
}

} // namespace

void test::sayr::plan() {
    using namespace mq::kernel;

    const auto made = fixture::make();
    require(made.has_value(), made.error_or("fixture failed"));
    const auto& fixture = *made;
    require(
        fixture.sayr.plan.size() == 6,
        "neutral sayr plan lost an obligation");

    const auto first = id("obligation.first");
    const auto second = id("obligation.second");
    const auto firstNeed = id("need.first");
    const auto secondNeed = id("need.second");
    const auto route = id("route");
    const std::vector<ks::Obligation> obligations{
        {
            first,
            {need(firstNeed, ks::need::Jins{fixture.jins.root})},
            {},
        },
        {
            second,
            {need(
                secondNeed,
                ks::need::Center{fixture.center.root})},
            {first},
        },
    };
    const std::vector<ks::Route> routes{
        {route, {second}},
    };
    const auto forward =
        ks::Plan::make(id("forward"), obligations, routes);
    auto reversed = obligations;
    std::ranges::reverse(reversed);
    const auto reverse =
        ks::Plan::make(id("reverse"), reversed, routes);
    require(
        forward && reverse,
        "valid sayr plan failed admission");

    const ks::Completion firstCompletion{
        sort::ObligationId{first},
        {{
            firstNeed,
            ks::proof::Jins{fixture.jins.root},
        }},
    };
    const ks::Completion secondCompletion{
        sort::ObligationId{second},
        {{
            secondNeed,
            ks::proof::Center{fixture.center.root},
        }},
    };
    const std::vector ordered{
        firstCompletion,
        secondCompletion,
    };
    const std::vector unordered{
        secondCompletion,
        firstCompletion,
    };
    require(
        forward->accepts(ordered) &&
            reverse->accepts(ordered) &&
            !forward->accepts(unordered),
        "sayr storage order or predecessor order changed acceptance");

    auto dangling = obligations;
    dangling.back().after = {id("missing")};
    require(
        !ks::Plan::make(id("dangling"), dangling, routes),
        "sayr plan accepted a dangling predecessor");

    auto cyclic = obligations;
    cyclic.front().after = {second};
    require(
        !ks::Plan::make(id("cyclic"), cyclic, routes),
        "sayr plan accepted a dependency cycle");

    auto invalid = obligations;
    invalid.front().needs.front().requirement =
        ks::need::Role{fixture.role.root, 0};
    require(
        !ks::Plan::make(id("invalid"), invalid, routes),
        "sayr plan accepted a zero-count landmark need");

    auto duplicated = obligations;
    duplicated.push_back(obligations.front());
    require(
        !ks::Plan::make(id("duplicate"), duplicated, routes),
        "sayr plan accepted a repeated obligation identity");

    auto badProof = ordered;
    badProof.back().proofs.front().value =
        ks::proof::Center{fixture.center.branch};
    require(
        !forward->accepts(badProof),
        "sayr route accepted a proof that contradicts its need");
}
