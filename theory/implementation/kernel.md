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
| Stable hard-constraint identities | uniqueness validation and identity-bearing contradiction traces |
| Exact one-tier contextual tendencies | active-set KKT equations solved through hard witnesses |
| Positive rational tendency weights | target validation and exact weighted-center laws |
| Hard-bound tendency interaction | upper, lower, and coupled active-set tests |
| Stable active-set traces | identity-normalized constraints and active identity output |
| Exact tier predictions | tendency-identity to affine prediction map |
| Explicit KKT resource failure | bounded active-set enumeration and nested hard-solver limits |
| Exact lexicographic tendency sequence | prediction-freeze equalities and repeated KKT tiers |
| Stronger-tier non-degradation | conflicting-target and null-space resolution laws |
| Stable tier and target identities | global uniqueness validation and trace steps |
| Explicit sequence resource failure | bounded tier count and per-tier nested limits |
| Guarded pitch-field assembly | exact context subset matching and identity normalization |
| Explicit contextual coverage | named rule sets with no fallback on a missing match |
| Named pitch authority | declared tier order and active empty-tier removal |
| Exact field integration | hard-only witness or full lexicographic tendency solve |
| Explicit field resource failure | bounded context, rule, coverage, and solver work |
| Typed state-to-field projection | center, jins, tonicization, and path readers |
| Required projection presence | explicit missing-state error or optional omission |
| Projection scope non-leakage | restored grammar outcome to field integration law |
| Explicit projection resource failure | bounded reader count |
| Structural pitch events | `performance::Event` and checked `Place` transition |
| Intended motion state | typed `Start`, `Same`, `Rise`, and `Fall` |
| Scoped event history | `Melody` state component and explicit scope export |
| Role and motion projection | typed readers over current structural event |
| Instrument-neutral target plan | exact event target prefix under `performance` |
| Plan-to-history correspondence | complete prefix and event identity validation |
| Exact motion verification | symbolic order certificates against prior target |
| Explicit motion proof failure | direction mismatch and order-budget laws |
| Explicit request resource failure | bounded event-history prefix validation |
| Profile rule values and provenance | `Rule`, `Profile` |
| Seven patch actions | reconstruction implementation and tests |
| Multiple-parent conflict rejection | profile law tests |
| Explicit conflict resolution | profile law tests |
| Regional narrowing, prohibition, and exception | profile law tests |
| Ten tonal, vocabulary, and event operators | `Operation` variant |
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
- Typed phrase-function and cadence state over structural event spans.
- Continuous intonation trajectory basis functions.
- A sourced real regional profile.

Audio rendering is outside this ledger. The independent synthesis boundary is
specified under `synthesis/`.

## Next coherent sequence

1. Define phrase spans over stable structural event identities.
2. Represent question, response, suspension, and closure functions explicitly.
3. Project phrase function and cadence state into calculated pitch requests.
4. Normalize `Alt` identities and test normalized associativity.
5. Add recursive production budgets before recursive grammar modules.
6. Encode one sourced Bayati evidence fixture only after its rule review.

## Validation environment

Verified in the current workspace:

- optimized GCC C++23 build with warnings as errors;
- twenty-four debug law suites;
- undefined-behavior sanitizer;

GCC AddressSanitizer starts LeakSanitizer in this container and then fails
because the traced environment denies its `/proc/<pid>/task` inspection. This
is an environment limitation rather than a test failure. `kernel-address`
remains a separate target for an untraced Linux environment; it is not counted
as passed here.
