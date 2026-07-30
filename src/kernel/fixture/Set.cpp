#include "Internal.hpp"

namespace mq::kernel::fixture {

std::expected<Set, std::string> make() {
    Set fixture{
        .center = {
            Identity{"fixture.center", "root", "1"},
            Identity{"fixture.center", "branch", "1"},
        },
        .jins = {
            Identity{"fixture.jins", "root", "1"},
            Identity{"fixture.jins", "branch", "1"},
        },
        .role = {
            Identity{"fixture.role", "root", "1"},
            Identity{"fixture.role", "ghammaz", "1"},
        },
        .phrase = {
            Identity{"fixture.phrase.function", "neutral", "1"},
        },
        .cell = Identity{"fixture.cell", "establish", "1"},
        .cadence = Identity{"fixture.cadence", "local", "1"},
        .path = {
            Identity{"fixture.path", "root-to-branch", "1"},
            {},
        },
        .profile = {},
    };
    const auto added = fixture.path.graph.add(path::Rule{
        fixture.path.direct,
        fixture.center.root,
        fixture.center.branch,
        tonicization::Level::Internal,
        {},
        "fixture:neutral-path",
    });
    if (!added) {
        return std::unexpected(added.error());
    }

    auto profiles = detail::profiles(fixture);
    if (!profiles) {
        return std::unexpected(profiles.error());
    }
    fixture.profile = std::move(*profiles);

    return fixture;
}

} // namespace mq::kernel::fixture
