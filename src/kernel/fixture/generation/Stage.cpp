#include "Internal.hpp"

namespace mq::kernel::fixture::generation::detail {

Identity id(std::string name) {
    return {"fixture.generate", std::move(name), "1"};
}

std::vector<generate::Stage> stages(
    const fixture::Set& fixture,
    std::string name,
    bool journey) {
    return journey
             ? travel(fixture, name)
             : std::vector<generate::Stage>{
                   stay(fixture, name),
               };
}

} // namespace mq::kernel::fixture::generation::detail
