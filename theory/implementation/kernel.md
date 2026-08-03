# Kernel implementation ledger

This ledger is the authority for what the C++23 theory kernel actually
implements. Historical prototype tests do not advance this ledger.

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
| Numeric cadence-strength consumption by contextual pitch rules | `pitch::field::assemble::Run` |
| Typed state-to-field projection | center, jins, tonicization, path, phrase, and event readers |
| Required projection presence | explicit missing-state error or optional omission |
| Projection scope non-leakage | restored grammar outcome to field integration law |
| Explicit projection resource failure | bounded reader count |
| Validated jins descriptor subset | catalog admission laws for tonic, ghammaz, role, register, baggage, and gesture references |
| Jins formula vocabulary | descriptor entry and exit formulas, cadence vocabulary, and motif lexicon |
| Missing descriptor rejection | entry and descriptor-dependent operations have no catalog fallback |
| Structural pitch events | descriptor-conformant `performance::Event` and checked `Place` transition |
| Intended motion state | typed `Start`, `Same`, `Rise`, and `Fall` |
| Relational event register and baggage | descriptor and effective-profile intersection laws |
| Ordered idiomatic gestures | descriptor-owned steps, active progress, and completed event spans |
| Scoped event history | `Melody` state component and explicit scope export |
| Gesture scope dependency | Gesture export requires Melody export |
| Event-context projection | role, motion, register, baggage, and gesture readers |
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
| Typed tonal, phrase, gesture, vocabulary, and event operators | `Operation` variant |
| Provenance-bearing descriptor overlay operations | `jins::Overlay` and tests |
| Exact tonicization evidence thresholds | `Evaluator` |
| Profile-declared choice bands beyond strict equal minimum-cost | `Engine::run` reads `band.generation` and `choice::select` accepts bands |
| Seeded lexicographic alternative selection | `choice::select` |
| Storage-order and ineligible-choice stability | choice law tests |
| Typed `Id`, `Fail`, `Seq`, `Alt`, `Guard`, `Repeat`, `Scope`, `Bind`, and `Produce` terms | `grammar::Term` |
| `Alt` identity normalization and its normalized associativity law | `Term::alt` and grammar relation tests |
| Recursive phrase production definitions and explicit well-founded budget accounting | `grammar::Catalog`, `grammar::Produce` term, and budget state |
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
| Multi-stage descriptor witness | root, ghammaz, baggage, return, and resolution gesture stages |
| Typed sayr obligations | dependency-ordered needs over jins, center, role, region, gesture, phrase, path, and evidence |
| Proof-bearing sayr fulfillment | checked completion history and retained typed proofs |
| Constrained sayr routes | terminal-obligation acceptance for stay and journey routes |
| Sayr scope restoration | explicit grammar scope component and non-leakage law |
| Complete candidate evaluation | legal outcomes and retained diagnostics before seed |
| Exact staged performance plans | one target per committed structural event, including exact `4:3` and `3:2` relations |
| Exact monophonic timed performance plan | `performance::Plan::events` derives rational onsets and validates duration, intensity, articulation, release, typed strand, optional exact pitch contours, ornament intent, and explicit pause spans |
| Profile-driven hierarchical execution timing | `performance::Timing` resolves phrase start/body/end rates, seeded bounded performer variation, release intent, and boundary pauses from external data; maqam packages and the shared family builder do not own fixed timing values |
| Profile-owned ornament host timing | Exact onset, active-duration, and release fractions are loaded with each direction intent and consumed by approach and oscillation adapters |
| External physical execution timing | Seconds-per-unit and render-tail values are required from the external timing profile rather than hard-coded in the renderer |
| Native corpus casebook validation | `validation::Casebook` parses evidence envelopes, exact observations, split coverage, pauses, releases, ornament coverage, and performer variation with an exact release gate |
| Executable profile comparison | reconstructed baggage prohibition changes the legal generation set |
| Typed grammar output | `Stage` and `Candidate` term forms |
| Scope-controlled generated output | explicit `Output` export and non-leakage law |
| Grammar-to-generator compilation | sealed derivations become exact candidate plans |
| Executable neutral generation witness | CLI seed selects a complete candidate and prints exact targets |
| Recursive phrase grammar | `grammar::Produce` and test-verified budget exhaustion |
| Contextual intonation basis | `pitch::trajectory` basis functions (onset, release, vibrato, attraction) |
| Typed maqām catalog and package admission | `maqam::Catalog`, typed `MaqamId`, explicit complete/incomplete entries, and refusal of incomplete or undeclared packages |
| Complete Rast package and routes | `src/kernel/maqam/rast`, `rast_case`, catalog admission, and CLI execution for stay, Upper Rast, and Nahawand routes |
| Complete Nahawand package and routes | `src/kernel/maqam/family`, `make_nahawand`, `nahawand_case`, catalog admission, and CLI execution for stay, Hijaz, and Kurd routes |
| Complete Hijaz package and routes | `make_hijaz`, `hijaz_case`, catalog admission, and CLI execution for stay, Nahawand, and Rast routes |
| Complete Kurd package and routes | `make_kurd`, `kurd_case`, catalog admission, and CLI execution for stay and Nahawand routes |
| Complete Ajam package and routes | `make_ajam`, `ajam_case`, catalog admission, and CLI execution for stay, Upper Ajam, and Nahawand routes |
| Complete Nikriz package and routes | `make_nikriz`, `nikriz_case`, catalog admission, and CLI execution for stay and Nahawand routes |
| Provisional Bayati profile and routes | Bayati collection record, generic family builder, `bayati_case`, and CLI execution for stay, Nahawand, Rast, and Hijaz routes; completion debt remains explicit |
| Declarative ordered-family seam | `family::BranchSpec` carries source/target centers, roles, and transition direction; the Sikah route exercises the shared builder without a route-name dispatch |
| Executable Sikah vertical route | `make_sikah`, `sikah_case`, named CLI execution, and native synthesis render; the catalog still rejects it as incomplete |

## Evidence still required

- A rights-cleared, profile-matched corpus for the Bayati regional profile.
- Human annotator review of structural layers.
- The checked-in casebook is a schema fixture keyed to AMICOR, not measured
  corpus data; profile-matched source segments and independent review are still
  required for phrase-rate, pause, release, ornament, and performer claims.
- A vocabulary-composition boundary for admitting a second theory module.
- Collection-file loading now supplies the executable family package records,
  including Bayati; the collection schema still needs the full phrase,
  ornament, timing, evidence, and variation vocabulary required for complete
  maqām packages.

Audio rendering is outside this ledger. The independent synthesis boundary is
implemented under `synthesis/`; its current renderer is a provisional
coupled-course model, not a named-instrument or regional-authenticity result.

## Next coherent sequence

1. Replace the engineering timing profile with performer/corpus-derived rate,
   pause, release, and ornament distributions while preserving exact symbolic
   plan laws. The casebook validator is now the ingestion and claim gate for
   that replacement; its AMICOR-keyed fixture is not a Bayati authenticity pass.
2. Validate and refine the coupled-course santur physical model against
   isolated-course and overlap measurements while preserving the one-way
   `performance::Plan::events` boundary.
3. Extend the provisional Bayati profile with reviewed phrase, ornament, and
   contextual intonation cases; do not treat the current route harness as
   regional authenticity validation.

## Validation environment

Verified for the preceding 34-suite descriptor checkpoint:

- optimized GCC C++23 build with warnings as errors;
- thirty-four debug law suites;
- undefined-behavior sanitizer;

The constrained checkout is not used for binary validation. GitHub Actions run
`30773498807` validates exact head
`dc9897247a1030eec72e698030e428763ec3f314` across debug, optimized CLI, Faust,
synthesis, and undefined-behavior jobs. The remote debug and UBSan jobs each
report 49 passing law/case invocations. The synthesis job renders deterministic
native and Faust-generated Sikah WAVs for 33.0256 seconds, and the Faust job
generates and compiles both pinned probe sources.
`.github/workflows/kernel.yml` is the clean remote validation path and installs
GCC 14 before running these checks.

GCC AddressSanitizer starts LeakSanitizer in this container and then fails
because the traced environment denies its `/proc/<pid>/task` inspection. This
is an environment limitation rather than a test failure. `kernel-address`
remains a separate target for an untraced Linux environment; it is not counted
as passed here.
