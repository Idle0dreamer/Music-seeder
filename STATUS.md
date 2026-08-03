# Project status

The old generator and audio artifacts remain retained for diagnosis. The
current kernel and separate provisional santur renderer are active
implementations; the evidence and limits for each are listed below. The model
is not a named-instrument result.

Current phase: **non-ML language and staged-generation kernel implementation**.

Kernel implementation is active only under `include/mq/kernel`, `src/kernel`,
`apps/kernel`, and `tests/kernel`. The separate `synthesis/` tree contains the
first one-way renderer boundary and a pinned Faust build probe; the C++ model
is provisional and not a named-instrument implementation.

Audio synthesis remains a separate subsystem under `synthesis/`. It is not a
kernel or theory-language dependency.

Implemented so far:

1. exact rational arithmetic;
2. symbolic prime-factor pitch expressions and exact affine solving;
3. proof-producing exact order over symbolic pitches;
4. exact hard affine inequality feasibility;
5. exact witnesses for underdetermined hard pitch regions;
6. stable hard-constraint identities and contradiction traces;
7. exact one-tier contextual tendency optimization through KKT conditions;
8. exact lexicographic tendency sequencing without weaker-tier degradation;
9. guarded contextual pitch-field assembly with explicit coverage;
10. end-to-end field solving through hard and ordered tendency systems;
11. typed projection of restored tonal, phrase, and structural-event state;
12. scoped grammar-to-calculated-field integration;
13. declarative profile reconstruction and conflict detection;
14. evidence-gated tonal operators;
15. stable seeded lexicographic choice;
16. typed finite grammar terms and complete relational evaluation;
17. lexical binding and explicit scope restoration;
18. path history with source, target, level, and prerequisite guards;
19. unique descriptor-conformant structural events with intended motion,
    relational register, baggage, gesture context, and scoped history;
20. role, direction, register, baggage, and gesture projection into contextual
    pitch fields;
21. exact instrument-neutral pitch targets with plan-prefix validation;
22. exact rational monophonic timed performance events with typed strands,
    duration, intensity, articulation, optional exact pitch contours, and
    ornament intent, with an explicit direction-conditioned timing profile
    supplied outside the maqam packages;
23. exact `Same`, `Rise`, and `Fall` order certificates;
24. event-bounded phrase spans with exact cadence evidence and strength;
25. typed phrase function, cadence, and boundary pitch projections;
26. exact phrase-conditioned structural pitch requests;
27. one-event generation stages composed into multi-stage candidates;
28. seeded selection only after every candidate reaches an exact plan or
    retained rejection;
29. effective-profile differences exercised through generated outcomes;
30. typed grammar `Stage` and `Candidate` output forms;
31. complete grammar-to-staged-plan generation with scope-controlled output;
32. seed-driven neutral CLI generation through the same tested path;
33. validated jins descriptor catalog with tonic, ghammaz, register, emphasis,
    baggage, and ordered gesture capabilities;
34. descriptor-gated entry, evidence, event placement, and exact register or
    baggage evidence;
35. ordered gesture progress, completed event spans, and explicit gesture
    scope restoration;
36. a five-stage neutral descriptor and sayr witness with exact calculated
    root, `4:3` ghammaz, `3:2` baggage, `4:3` return, and root resolution
    targets;
37. executable profile narrowing that removes that travel candidate at its
    baggage step while retaining the diagnostic;
38. typed sayr plans, dependency-ordered obligations, proof-bearing
    fulfillment, route completion, and scope restoration;
39. fifty-one registered executable test-function invocations (48 summary
    lines);
40. typed maqam catalog with explicit complete/incomplete admission and
    undeclared-package refusal;
41. complete canonical Rast package with Upper Rast and Nahawand routes,
    contextual targets, timed plans, and CLI execution;
42. complete canonical Nahawand package with Hijaz and Kurd routes,
    contextual targets, timed plans, and CLI execution;
43. complete canonical Hijaz package with Nahawand and Rast routes,
    contextual targets, timed plans, and CLI execution;
44. complete canonical Kurd package with a Nahawand route, contextual targets,
    timed plans, and CLI execution;
45. complete canonical Ajam package with Upper Ajam and Nahawand routes,
    contextual targets, timed plans, and CLI execution;
46. complete canonical Nikriz package with a Nahawand route, contextual
    targets, timed plans, and CLI execution;
47. a sourced, explicitly provisional Bayati profile with root Bayati,
    Nahawand, Rast, and Hijaz upper-jins routes;
48. positive sayr completion laws and seeded grammar derivations for all four
    Bayati routes;
49. branch-conditioned exact pitch targets rather than route-name-only
    variation;
50. a separate C++ synthesis target with a provisional coupled-course santur
    model that consumes timed plans and writes deterministic mono PCM WAV;
51. a Faust-generated C++ renderer target with exact-contour and oscillation
    frequency updates, intensity, strike, and neutral/connected/detached
    attack-force mapping;
52. an external timing-profile parser carries exact rational durations,
    intensities, articulation, phrase-rate selection, seeded bounded variation,
    explicit release intent, and boundary pauses through generation;
53. ornament host timing is an explicit profile fraction and is realized by
    both approach and oscillation pitch adapters;
54. render seconds-per-unit and tail duration are required external execution
    profile values rather than renderer defaults;
55. native C++ casebook parsing and exact timing/release/ornament/pause/
    performer-variation validation with provenance and disagreement fields;
56. synthesis consumes explicit release spans in the independent coupled-course
    model while preserving the one-way instrument-neutral plan boundary;
57. declarative ordered-family branch records carry source/target centers,
    tonic and endpoint roles, and transition direction without route-name
    conditionals;
58. an executable Sikah ordered-station vertical route with direct kernel and
    native WAV checks, retained outside the complete-package catalog gate;
59. one synthesis-side maqam registry feeds both native and Faust renderers;
60. minimal flag-based synthesis/player configuration for maqam, seed, timing
    profile, output path, sample rate, and tonic frequency;
61. Faust release-duration control is consumed from each timed event rather
    than silently using a renderer timing default;
62. configurable multi-phrase rendering with a duration target, deterministic
    seeded continuation phrases, and continuous player looping;
63. a Qt desktop generator/player with asynchronous rendering, WAV waveform
    inspection, and soft Fourier magnitude inspection;
64. prepared native santur voices with cached pitch/strike geometry, modal
    wavetable lookup, and bounded audible-tail mixing for long renders;
65. block-oriented Faust execution with persistent generated-DSP state between
    audio blocks while preserving one-sample strike edges;
66. long performance generation now uses a separate external continuous
    timing profile for bounded performer variation and rotates through every
    complete legal collection route available to each continuation;

Checkpoint validation state:

- GitHub Actions run `30774537059` validates exact head
  `b2f6908e8c7d1c0585e016739f0a4f1e7f83b295`; its debug law suite reports 49
  passing law/case invocations, and its optimized CLI job exercises the
  neutral fixture and named Sikah route remotely;
- the same run passes the UBSan suite with 49 passing law/case invocations;
- the remote synthesis job renders both native and Faust-generated coupled-
  course models as non-silent RIFF WAVs: 1,585,230 frames, 33.0256 seconds,
  peak `0.92`; the repeated Faust render is byte-identical;
- the Faust job builds the pinned compiler, generates both physical-modeling
  and coupled-course C++ sources, and compiles both generated probes;
- run `30774537059` uploads `music-seeder-kernel-binaries` (`8841668945`),
  `music-seeder-sanitizer-binary` (`8841656343`),
  `music-seeder-faust-compiler` (`8841643712`),
  `music-seeder-player-binaries` (`8841794875`), and
  `music-seeder-sikah-wav` (`8841794404`);
- those remote binaries were downloaded to
  `output/remote-binaries-b2f6908` and executed locally without compiling:
  the kernel and UBSan suites each produced 49 passes, both renderers produced
  valid 48 kHz WAVs, the local Faust render matched the remote Faust artifact
  byte-for-byte, and the Qt player remained running on the local display;
- `.github/workflows/kernel.yml` provides the free remote debug, release, CLI,
  Faust, synthesis, and undefined-behavior validation path on GitHub Actions;
- AddressSanitizer remains a separate environmental check and is not counted
  as passed here.

The Bayati profile is an explicitly provisional Egypt-Levant research profile,
not a claim of authentic regional or school-specific performance. The
Nahawand package is scoped to the canonical repertoire description and does not
claim every regional, historical, performer, or school realization. Bayati's
source scope and remaining corpus/annotator debt are recorded in
`theory/implementation/kernel.md` and `theory/case/bayati.md`.

The neutral fixture remains finite and intentionally limited. The Bayati
profile now executes four finite sourced route candidates with onset-to-target
pitch contours, but it is not yet a recursive phrase, motif, or ornament
generator, and its sayr vocabulary is not culturally complete. The santur
renderer is an unvalidated provisional physical model, not a regional or
instrument-authentic claim.

The normative model remains under `theory/`.
