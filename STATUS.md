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
    duration, intensity, and articulation intent;
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
39. forty-two registered executable test-function invocations (39 summary
    lines).
40. a sourced, explicitly provisional Bayati profile with root Bayati,
    Nahawand, Rast, and Hijaz upper-jins routes;
41. positive sayr completion laws and seeded grammar derivations for all four
    Bayati routes;
42. branch-conditioned exact pitch targets rather than route-name-only
    variation;
43. a separate C++ synthesis target with a provisional coupled-course santur
    model that consumes timed plans and writes deterministic mono PCM WAV;

Checkpoint validation state:

- the current working tree passes `make kernel-test` with GCC debug flags,
  all 42 test-function invocations, and 39 summary lines;
- the current working tree passes `make kernel` with `-O2 -Werror`;
- `./build/kernel 1` emits the five-event neutral travel candidate with exact
  onsets `0`, `1`, `2`, `3`, and `4`; `./build/kernel 2` emits the one-event
  neutral stay candidate;
- `./build/kernel bayati 17` emits one selected five-event Bayati route from
  four legal routes, with branch-conditioned exact pitch targets;
- `make synthesis` and `./build/synthesis-render 17 /tmp/bayati.wav` produce a
  non-silent valid 48 kHz PCM WAV from the provisional coupled-course model;
  repeated renders have the same SHA-256;
- `.github/workflows/kernel.yml` now compiles the pinned Faust-generated C++
  probe and exercises the synthesis render online; this workflow has not yet
  run for the current unpushed tree;
- this evidence is for the current working tree, not a claim that pristine
  remote commit `310cfa8` contains the uncommitted fixes;
- `.github/workflows/kernel.yml` provides the free remote debug, release, CLI,
  and undefined-behavior validation path on GitHub Actions;
- AddressSanitizer remains a separate environmental check and is not counted
  as passed here.

The Bayati profile is an explicitly provisional Egypt-Levant research profile,
not a claim of authentic regional or school-specific performance. Its source
scope and remaining corpus/annotator debt are recorded in
`theory/implementation/kernel.md` and `theory/case/bayati.md`.

The neutral fixture remains finite and intentionally limited. The Bayati
profile now executes four finite sourced route candidates, but it is not yet a
recursive phrase, motif, phrase-conditioned timing, or ornament generator, and
its sayr vocabulary is not culturally complete. The santur renderer is an
unvalidated provisional physical model, not a regional or instrument-authentic
claim.

The normative model remains under `theory/`.
