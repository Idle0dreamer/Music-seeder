#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/pitch/Expression.hpp"

#include <expected>
#include <map>
#include <string>
#include <vector>

namespace mq::kernel::pitch {

struct Term {
    Identity variable;
    Rational coefficient;
};

struct Equation {
    std::vector<Term> terms;
    Expression right;
    std::string provenance;
};

struct Solution {
    std::map<Identity, Expression> values;
};

class System {
public:
    void add(Identity variable);
    void add(Equation equation);

    [[nodiscard]] std::expected<Solution, std::string> solve() const;

private:
    std::vector<Identity> variables_;
    std::vector<Equation> equations_;
};

} // namespace mq::kernel::pitch
