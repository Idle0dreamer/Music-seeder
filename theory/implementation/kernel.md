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
| Twelve tonal, phrase, vocabulary, and event operators | `Operation` variant |
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
| Event-bounded phrase spans | checked `Begin`, `Cadence`, and `End` transitions |
| Exact cadence evidence and closure strength | distinct evidence and span fields |
| Final-event closed-phrase rule | phrase transition laws |
| Phrase scope dependency | Phrase export requires Melody export |
| Typed phrase pitch context | function, cadence family, and boundary readers |
| Phrase-conditioned exact pitch target | end-to-end request law |
| One-event generation stages | static and runtime stage-count laws |
| Complete candidate evaluation | legal outcomes and retained diagnostics before seed |
| Exact staged performance plans | one target per committed structural event |
| Executable profile comparison | reconstructed prohibition changes legal generation set |
| Typed grammar output | `Stage` and `Candidate` term forms |
| Scope-controlled generated output | explicit `Output` export and non-leakage law |
| Grammar-to-generator compilation | sealed derivations become exact candidate plans |
| Executable neutral generation witness | CLI seed selects a complete candidate and prints exact targets |

## Deliberately not claimed yet

- Phrase relations, motif, sayr, ornament, timing, and iqa operator families.
- `Alt` identity normalization and its normalized associativity law.
- Recursive production definitions and explicit well-founded budget accounting.
- Profile-declared choice bands beyond equal minimum-cost outcomes.
- Numeric cadence-strength consumption by contextual pitch rules.
- A full jins melodic-unit descriptor and its baggage or gesture grammar.
- A grammar that constructs extended musical candidates rather than neutral
  finite fixtures.
- Continuous intonation trajectory basis functions.
- A sourced real regional profile.
- A vocabulary-composition boundary for admitting a second theory module.

Audio rendering is outside this ledger. The independent synthesis boundary is
specified under `synthesis/`.

## Next coherent sequence

1. Define a full jins melodic-unit descriptor without scale arrays: stable
   tonic and ghammaz roles, register regions, emphasis roles, baggage
   capabilities, and gesture identities.
2. Consume that descriptor in event-stage legality and pitch projection.
3. Define sayr obligations over completed phrase spans, jins state, and path
   history.
4. Consume sayr obligations in candidate legality before adding recursive
   productions.
5. Add explicit well-founded production budgets, then recursive phrase
   production definitions.
6. Admit sourced regional evidence only after the corresponding generated
   consequences have validation laws.

## Validation environment

Verified in the current workspace:

- optimized GCC C++23 build with warnings as errors;
- thirty debug law suites;
- undefined-behavior sanitizer;

GCC AddressSanitizer starts LeakSanitizer in this container and then fails
because the traced environment denies its `/proc/<pid>/task` inspection. This
is an environment limitation rather than a test failure. `kernel-address`
remains a separate target for an untraced Linux environment; it is not counted
as passed here.
