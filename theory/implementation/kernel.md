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
| Executable profile comparison | reconstructed baggage prohibition changes the legal generation set |
| Typed grammar output | `Stage` and `Candidate` term forms |
| Scope-controlled generated output | explicit `Output` export and non-leakage law |
| Grammar-to-generator compilation | sealed derivations become exact candidate plans |
| Executable neutral generation witness | CLI seed selects a complete candidate and prints exact targets |
| Recursive phrase grammar | `grammar::Produce` and test-verified budget exhaustion |
| Contextual intonation basis | `pitch::trajectory` basis functions (onset, release, vibrato, attraction) |
| Regional profile test harness | `bayati_case` harness matching the Egyptian/Levantine case specification |

## Deliberately not claimed yet

- A rights-cleared, profile-matched corpus for the Bayati regional profile.
- Human annotator review of structural layers.
- A vocabulary-composition boundary for admitting a second theory module.

Audio rendering is outside this ledger. The independent synthesis boundary is
specified under `synthesis/`.

## Next coherent sequence

1. Finish remote validation of the typed neutral sayr checkpoint and record
   executable evidence.
2. Admit sourced regional evidence only after the corresponding generated
   consequences have validation laws.

## Validation environment

Verified for the preceding 34-suite descriptor checkpoint:

- optimized GCC C++23 build with warnings as errors;
- thirty-four debug law suites;
- undefined-behavior sanitizer;

The current sayr code completed a clean remote GCC 14 debug build, linked, and
passed all 37 registered suite entry points. Optimized compilation, sanitizers,
and CLI runs remain unverified. A separate Modal attempt inherited ignored
desktop build objects, so its partial release and sanitizer results are not
evidence.

Use `tools/notebook/Build.ipynb` on an online CPU runtime to complete the
remaining evidence without desktop memory pressure. It installs and probes
GCC 14, forces that compiler for every target, and validates a clean clone.

GCC AddressSanitizer starts LeakSanitizer in this container and then fails
because the traced environment denies its `/proc/<pid>/task` inspection. This
is an environment limitation rather than a test failure. `kernel-address`
remains a separate target for an untraced Linux environment; it is not counted
as passed here.
