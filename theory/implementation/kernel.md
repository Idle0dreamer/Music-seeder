# Kernel implementation ledger

This ledger is the authority for what the C++23 theory kernel actually
implements. Passing legacy prototype tests does not advance this ledger.

## Implemented

| Capability | Evidence |
| --- | --- |
| Reduced exact rational arithmetic | `Rational` and arithmetic law tests |
| Stable scoped identities | `Identity` |
| Symbolic ratio pitch expressions | prime-exponent representation |
| Exact affine pitch equalities | rational Gaussian elimination |
| Contradiction and underdetermination rejection | pitch solver tests |
| Profile rule values and provenance | `Rule`, `Profile` |
| Seven patch actions | reconstruction implementation and tests |
| Multiple-parent conflict rejection | profile law tests |
| Explicit conflict resolution | profile law tests |
| Regional narrowing, prohibition, and exception | profile law tests |
| Nine tonal/vocabulary operators | `Operation` variant |
| Exact tonicization evidence thresholds | `Evaluator` |
| Seeded lexicographic alternative selection | `Choice` |
| Storage-order and ineligible-choice stability | choice law tests |

## Deliberately not claimed yet

- Full grammar term tree for `Id`, `Fail`, `Seq`, `Alt`, `Guard`, `Repeat`,
  `Scope`, and `Bind`.
- Relational evaluation that preserves multiple complete derivations.
- Phrase, motif, cadence, sayr, ornament, timing, and iqa operator families.
- Path-conditioned modulation prerequisites beyond named path permission.
- General affine inequalities.
- Lexicographically ordered soft pitch-constraint tiers.
- Continuous intonation trajectory basis functions.
- A sourced real regional profile.
- A replacement dulcimer renderer.

## Next coherent sequence

1. Implement the grammar term tree and bounded evaluator.
2. Make `Alt` evaluate complete legal branches before seeded selection.
3. Implement scope restoration and non-leakage tests.
4. Add path-state and prerequisite guards.
5. Extend pitch solving with hard inequalities and ordered soft tiers.
6. Encode one sourced Bayati evidence fixture only after its rule review.

## Validation environment

Verified in the current workspace:

- optimized GCC C++23 build with warnings as errors;
- debug law suite;
- undefined-behavior sanitizer;
- coexistence build and tests for the isolated legacy prototype.

GCC AddressSanitizer starts LeakSanitizer in this container and then fails
because the traced environment denies its `/proc/<pid>/task` inspection. This
is an environment limitation rather than a test failure. `kernel-address`
remains a separate target for an untraced Linux environment; it is not counted
as passed here.
