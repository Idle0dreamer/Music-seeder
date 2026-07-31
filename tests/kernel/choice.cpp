#include "Test.hpp"

#include "mq/kernel/choice/Select.hpp"

#include <algorithm>
#include <vector>

void test::choice() {
    using namespace mq::kernel;
    const Identity expression{"test.grammar", "alternative", "1"};
    const choice::Candidate first{
        Identity{"test.cell", "first", "1"},
        choice::Cost{{0, 0, 0, 0}},
        true,
    };
    const choice::Candidate second{
        Identity{"test.cell", "second", "1"},
        choice::Cost{{0, 0, 0, 0}},
        true,
    };
    const choice::Candidate forbidden{
        Identity{"test.cell", "forbidden", "1"},
        choice::Cost{{-100, -100, -100, -100}},
        false,
    };

    const std::vector<Identity> scope{
        Identity{"test.section", "0", "1"},
        Identity{"test.phrase", "1", "1"},
    };
    std::vector<choice::Candidate> candidates{first, second};
    const auto selected =
        choice::select(44, expression, scope, candidates);
    require(selected.has_value(), selected.error_or("choice failed"));

    std::ranges::reverse(candidates);
    require(
        choice::select(44, expression, scope, candidates) == selected,
        "storage order changed seeded choice");

    candidates.push_back(forbidden);
    require(
        choice::select(44, expression, scope, candidates) == selected,
        "ineligible alternative changed seeded choice");

    bool varies = false;
    for (std::uint64_t seed = 45; seed < 256; ++seed) {
        const auto other =
            choice::select(seed, expression, scope, candidates);
        if (other && *other != *selected) {
            varies = true;
            break;
        }
    }
    require(varies, "seed could not vary equally ranked alternatives");

    const choice::Candidate preferred{
        Identity{"test.cell", "preferred", "1"},
        choice::Cost{{0, 0, 0, 0}},
        true,
    };
    const choice::Candidate costly{
        Identity{"test.cell", "costly", "1"},
        choice::Cost{{0, 1, 0, 0}},
        true,
    };
    const std::vector ranked{costly, preferred};
    for (std::uint64_t seed = 0; seed < 32; ++seed) {
        require(
            choice::select(seed, expression, scope, ranked) ==
                std::expected<Identity, std::string>(preferred.identity),
            "seed overrode a stronger cost tier");
    }

    // Choice band allows picking the costly candidate if it is within the band.
    const choice::Cost band{{0, 1, 0, 0}};
    bool band_picked_costly = false;
    for (std::uint64_t seed = 0; seed < 64; ++seed) {
        const auto result = choice::select(seed, expression, scope, ranked, band);
        require(result.has_value(), result.error_or("choice band failed"));
        if (*result == costly.identity) {
            band_picked_costly = true;
            break;
        }
    }
    require(band_picked_costly, "choice band did not permit selecting costly candidate");
}
