# Kernel implementation ledger

This ledger is the authority for what the C++23 theory kernel actually
implements. Passing legacy prototype tests does not advance this ledger.

## Implemented

| Capability | Evidence |
| --- | --- |
| Reduced exact rational arithmetic | `Rational` and arithmetic law tests |
| Stable scoped identities | `Identity` |
| Symbolic ratio pitch expressions | prime-exponent representation |
| Exact affine hard constraints | rational equalities and closed inequalities |
| Exact symbolic pitch order | arbitrary-precision integer certificates and order laws |
| Bounded certificate verification | deterministic signed-64-bit prime validation and typed proof limits |
| General hard affine inequality feasibility | Fourier--Motzkin projection laws |
| Infeasibility proof and provenance | constant order certificate and merged constraint sources |
| Explicit projection resource failure | typed row, arithmetic, and order-proof errors |
| Exact underdetermined witnesses | retained stages, reverse substitution, and source-constraint validation |
| Unified hard solver authority | equalities and inequalities use the same feasibility/witness path |
| Profile rule values and provenance | `Rule`, `Profile` |
| Seven patch actions | reconstruction implementation and tests |
| Multiple-parent conflict rejection | profile law tests |
| Explicit conflict resolution | profile law tests |
| Regional narrowing, prohibition, and exception | profile law tests |
| Nine tonal/vocabulary operators | `Operation` variant |
| Exact tonicization evidence thresholds | `Evaluator` |
| Seeded lexicographic alternative selection | `choice::select` |
| Storage-order and ineligible-choice stability | choice law tests |
| Typed `Id`, `Fail`, `Seq`, `Alt`, `Guard`, `Repeat`, `Scope`, and `Bind` terms | `grammar::Term` |
| Complete finite legal derivations before seed resolution | grammar relation tests |
| Rejected-branch diagnostics retained beside legal outcomes | grammar relation tests |
| Lexical binding restoration | grammar non-leakage tests |
| Explicit state projections and scope exports | scope restoration tests |
| Typed path source, target, maximum level, and prior-path guards | path graph tests |
| Missing path graph rejects modulation without fallback | path guard tests |

## Deliberately not claimed yet

- Phrase, motif, cadence, sayr, ornament, timing, and iqa operator families.
- `Alt` identity normalization and its normalized associativity law.
- Recursive production definitions and explicit well-founded budget accounting.
- Profile-declared choice bands beyond equal minimum-cost outcomes.
- Lexicographically ordered soft pitch-constraint tiers.
- Continuous intonation trajectory basis functions.
- A sourced real regional profile.
- A replacement dulcimer renderer.

## Next coherent sequence

1. Consume hard witnesses in active-set KKT solving for the first soft tier.
2. Preserve stronger-tier predictions before optimizing later tiers.
3. Prove active-set and storage-order invariance.
4. Normalize `Alt` identities and test normalized associativity.
5. Add recursive production budgets before recursive grammar modules.
6. Encode one sourced Bayati evidence fixture only after its rule review.

## Validation environment

Verified in the current workspace:

- optimized GCC C++23 build with warnings as errors;
- eleven debug law suites;
- undefined-behavior sanitizer;

GCC AddressSanitizer starts LeakSanitizer in this container and then fails
because the traced environment denies its `/proc/<pid>/task` inspection. This
is an environment limitation rather than a test failure. `kernel-address`
remains a separate target for an untraced Linux environment; it is not counted
as passed here.
