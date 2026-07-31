#pragma once

#include "mq/kernel/Identity.hpp"

#include <string>
#include <utility>

namespace mq::kernel::sort {

template<typename Tag>
struct TypedId {
    Identity identity;

    TypedId() = default;
    explicit TypedId(Identity id) : identity(std::move(id)) {}
    explicit TypedId(std::string d, std::string n, std::string r = "1") 
        : identity{std::move(d), std::move(n), std::move(r)} {}

    auto operator<=>(const TypedId&) const = default;
    bool operator==(const TypedId&) const = default;

    std::string str() const { return identity.str(); }
};

} // namespace mq::kernel::sort
