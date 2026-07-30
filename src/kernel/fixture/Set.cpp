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
            Identity{"fixture.role", "extension", "1"},
        },
        .region = {
            Identity{"fixture.register", "root", "1"},
            Identity{"fixture.register", "upper", "1"},
        },
        .gesture = {
            Identity{"fixture.gesture", "ascent", "1"},
            Identity{"fixture.gesture", "descent", "1"},
        },
        .baggage = Identity{"fixture.baggage", "extension", "1"},
        .phrase = {
            Identity{"fixture.phrase.function", "neutral", "1"},
        },
        .cell = Identity{"fixture.cell", "establish", "1"},
        .cadence = Identity{"fixture.cadence", "local", "1"},
        .path = {
            Identity{"fixture.path", "root-to-branch", "1"},
            {},
        },
        .sayr = {
            .obligation = {
                Identity{"fixture.sayr.obligation", "establish", "1"},
                Identity{"fixture.sayr.obligation", "settle", "1"},
                Identity{"fixture.sayr.obligation", "expand", "1"},
                Identity{"fixture.sayr.obligation", "climax", "1"},
                Identity{"fixture.sayr.obligation", "travel", "1"},
                Identity{"fixture.sayr.obligation", "restore", "1"},
            },
            .route = {
                Identity{"fixture.sayr.route", "stay", "1"},
                Identity{"fixture.sayr.route", "journey", "1"},
            },
            .plan = {},
        },
        .catalog = {},
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

    auto catalog = detail::catalog(fixture);
    if (!catalog) {
        return std::unexpected(catalog.error());
    }
    fixture.catalog = std::move(*catalog);

    auto sayr = detail::sayr(fixture);
    if (!sayr) {
        return std::unexpected(sayr.error());
    }
    fixture.sayr.plan = std::move(*sayr);

    auto profiles = detail::profiles(fixture);
    if (!profiles) {
        return std::unexpected(profiles.error());
    }
    fixture.profile = std::move(*profiles);

    return fixture;
}

} // namespace mq::kernel::fixture
