#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

pitch::field::project::Plan projection(
    const Key& key) {
    namespace pp = pitch::field::project;

    return {{
        pp::role::Read{key.role},
        pp::motion::Read{
            key.motion,
            key.start,
            key.same,
            key.rise,
            key.fall,
        },
        pp::registral::Read{key.region},
        pp::baggage::Read{key.baggage, pp::Presence::Optional},
        pp::gesture::Read{key.gesture, pp::Presence::Optional},
        pp::phrase::function::Read{key.function},
        pp::phrase::cadence::Read{
            key.cadence,
            pp::Presence::Optional,
        },
        pp::phrase::boundary::Read{
            key.boundary,
            key.open,
            key.closed,
            pp::Presence::Optional,
        },
    }};
}

} // namespace mq::kernel::fixture::generation::detail
