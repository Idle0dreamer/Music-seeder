#include "Test.hpp"

#include "mq/kernel/math/Natural.hpp"
#include "mq/kernel/pitch/order/Compare.hpp"

void test::order() {
    using namespace mq::kernel;
    namespace po = pitch::order;

    const auto fourth = pitch::Expression::ratio(4, 3);
    const auto cents = pitch::Expression::cents(Rational(498));
    const auto proof = po::compare(fourth, cents);
    require(proof.has_value(), "exact pitch comparison failed");
    require(
        proof->relation == po::Relation::Greater &&
            proof->scale == 200 &&
            proof->normalized.at(2) == Rational(317, 200) &&
            proof->normalized.at(3) == Rational(-1),
        "4:3 versus 498 cents produced the wrong certificate");
    const auto verified = po::verify(*proof);
    require(
        verified && *verified,
        "pitch order certificate did not reconstruct");

    const auto reversed = po::compare(cents, fourth);
    require(
        reversed && reversed->relation == po::Relation::Less,
        "pitch order was not antisymmetric");
    const auto equal = po::compare(
        pitch::Expression::ratio(4, 3),
        pitch::Expression::ratio(8, 6));
    require(
        equal && equal->relation == po::Relation::Equal,
        "equivalent ratios did not compare equal");

    const auto shift = pitch::Expression::cents(Rational(31, 2));
    const auto translated = po::compare(fourth + shift, cents + shift);
    require(
        translated && translated->relation == proof->relation,
        "common pitch translation changed ordering");
    const auto lower = po::compare(
        cents,
        pitch::Expression::cents(Rational(497)));
    require(
        lower && lower->relation == po::Relation::Greater,
        "rational-cent transitivity premise failed");
    const auto transitive = po::compare(
        fourth,
        pitch::Expression::cents(Rational(497)));
    require(
        transitive && transitive->relation == po::Relation::Greater,
        "pitch order was not transitive");

    const auto comma = po::compare(
        pitch::Expression::ratio(3, 2) * Rational(12),
        pitch::Expression::ratio(2, 1) * Rational(7));
    require(
        comma && comma->relation == po::Relation::Greater,
        "Pythagorean comma sign was lost");

    const auto limited = po::compare(
        pitch::Expression::ratio(3, 2) * Rational(12),
        pitch::Expression::ratio(2, 1) * Rational(7),
        po::Limits{10, 1024});
    require(
        !limited && limited.error().code == po::Error::Code::Exponent,
        "proof budget silently approximated a large exponent");

    auto altered = *proof;
    altered.positive = "0";
    const auto rejected = po::verify(altered);
    require(
        rejected && !*rejected,
        "altered order certificate was accepted");

    const auto power = math::power(math::Natural(2), 64, 128);
    require(
        power && power->str() == "18446744073709551616",
        "arbitrary-precision integer power is incorrect");
    const auto square =
        math::Natural(18'446'744'073'709'551'615ULL) *
        math::Natural(18'446'744'073'709'551'615ULL);
    require(
        square.str() == "340282366920938463426481119284349108225",
        "arbitrary-precision carry propagation is incorrect");
    require(
        !math::power(math::Natural(2), 64, 64),
        "arbitrary-precision bit budget was exceeded silently");

    po::Proof composite{
        po::Relation::Greater,
        {{4, Rational(1)}},
        1,
        "4",
        "1",
    };
    const auto invalid = po::verify(composite);
    require(
        !invalid && invalid.error().code == po::Error::Code::Prime,
        "composite pitch basis was accepted as prime");
}
