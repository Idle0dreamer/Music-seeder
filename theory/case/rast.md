# Rast case study specification

This package implements the canonical Arabic Maqam Rast entry within the
declared MaqamWorld repertoire boundary. It is a distinct profile, not a
Bayati alias or a scale-table substitution.

## Sourced structural authority

The package follows the published Rast description: root Jins Rast on the
tonic, with an upper Jins Upper Rast or Jins Nahawand branch beginning at the
ghammaz. The implementation keeps those as typed descriptors and separate
path identities. See [Maqam Rast](https://www.maqamworld.com/en/maqam/rast.php),
[Jins Rast](https://maqamworld.com/en/jins/rast.php), and the
[Rast family index](https://www.maqamworld.com/en/maqam/f_rast.php).

## Implemented package contract

- root Rast descriptor with tonic, ghammaz, upper register, baggage, and
  ascent/descent/resolution gestures;
- independent Upper Rast and Nahawand descriptors;
- positive internal paths for both documented upper branches;
- dependency-checked establishment, expansion, climax, travel, and return
  obligations;
- stay, Upper Rast, and Nahawand complete candidates;
- question/response phrase boundaries, cadence strengths, repeated-cell
  variation, contextual symbolic targets, and exact timing intents;
- seeded CLI generation through `kernel rast [seed]`;
- law test coverage for all three routes and the shared typed catalog.

The package scope is canonical Arabic repertoire v1. It does not silently
claim every Turkish, Iraqi, regional, historical, or performer-specific Rast
practice. Those are separate profile identities and must add their own
evidence before admission.

## Negative authority

Only the two documented upper branch paths are admitted. A compatible jins or
pitch target without one of those path identities is not a legal Rast
modulation. Missing descriptors and missing paths remain hard errors.
