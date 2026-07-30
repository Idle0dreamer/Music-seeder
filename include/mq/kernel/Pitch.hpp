#pragma once

#include "mq/kernel/Identity.hpp"
#include "mq/kernel/Rational.hpp"

#include <expected>
#include <map>
#include <string>
#include <vector>

namespace mq::kernel {

class PitchExpression {
public:
    static PitchExpression cents(Rational value);
    static PitchExpression ratio(std::int64_t numerator, std::int64_t denominator);

    [[nodiscard]] const Rational& centOffset() const noexcept;
    [[nodiscard]] const std::map<std::int64_t, Rational>& primePowers() const noexcept;
    [[nodiscard]] double toCents() const;
    [[nodiscard]] std::string str() const;

    PitchExpression& operator+=(const PitchExpression& other);
    PitchExpression& operator-=(const PitchExpression& other);
    PitchExpression& operator*=(const Rational& scale);

    friend PitchExpression operator+(
        PitchExpression left,
        const PitchExpression& right) {
        return left += right;
    }

    friend PitchExpression operator-(
        PitchExpression left,
        const PitchExpression& right) {
        return left -= right;
    }

    friend PitchExpression operator*(
        PitchExpression left,
        const Rational& scale) {
        return left *= scale;
    }

    friend bool operator==(const PitchExpression&, const PitchExpression&) = default;

private:
    Rational cents_{};
    std::map<std::int64_t, Rational> primePowers_;

    void normalize();
};

struct PitchTerm {
    Identity variable;
    Rational coefficient;
};

struct PitchEquation {
    std::vector<PitchTerm> terms;
    PitchExpression right;
    std::string provenance;
};

struct PitchSolution {
    std::map<Identity, PitchExpression> values;
};

class PitchSystem {
public:
    void addVariable(Identity variable);
    void addEquation(PitchEquation equation);

    [[nodiscard]] std::expected<PitchSolution, std::string> solve() const;

private:
    std::vector<Identity> variables_;
    std::vector<PitchEquation> equations_;
};

} // namespace mq::kernel

