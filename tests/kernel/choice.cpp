#include "Test.hpp"

#include "mq/kernel/Choice.hpp"

#include <algorithm>
#include <vector>

void choiceTests() {
    using namespace mq::kernel;
    const Identity expression{"test.grammar", "alternative", "1"};
    const Candidate first{
        Identity{"test.cell", "first", "1"},
        Cost{{0, 0, 0, 0}},
        true,
    };
    const Candidate second{
        Identity{"test.cell", "second", "1"},
        Cost{{0, 0, 0, 0}},
        true,
    };
    const Candidate forbidden{
        Identity{"test.cell", "forbidden", "1"},
        Cost{{-100, -100, -100, -100}},
        false,
    };

    std::vector<Candidate> candidates{first, second};
    const auto selected =
        select(44, expression, "section/0/phrase/1", candidates);
    require(selected.has_value(), selected.error_or("choice failed"));

    std::ranges::reverse(candidates);
    require(
        select(44, expression, "section/0/phrase/1", candidates) == selected,
        "storage order changed seeded choice");

    candidates.push_back(forbidden);
    require(
        select(44, expression, "section/0/phrase/1", candidates) == selected,
        "ineligible alternative changed seeded choice");

    bool seedCanVary = false;
    for (std::uint64_t seed = 45; seed < 256; ++seed) {
        const auto other =
            select(seed, expression, "section/0/phrase/1", candidates);
        if (other && *other != *selected) {
            seedCanVary = true;
            break;
        }
    }
    require(seedCanVary, "seed could not vary equally ranked alternatives");

    const Candidate preferred{
        Identity{"test.cell", "preferred", "1"},
        Cost{{0, 0, 0, 0}},
        true,
    };
    const Candidate costly{
        Identity{"test.cell", "costly", "1"},
        Cost{{0, 1, 0, 0}},
        true,
    };
    const std::vector ranked{costly, preferred};
    for (std::uint64_t seed = 0; seed < 32; ++seed) {
        require(
            select(seed, expression, "scope", ranked) ==
                std::expected<Identity, std::string>(preferred.identity),
            "seed overrode a stronger cost tier");
    }
}

