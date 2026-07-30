# Project status

The previous generator executable and audio output are rejected exploratory
prototypes. They are not included in this checkpoint and are not evidence of a
completed maqam generator or audio renderer.

Current phase: **non-ML language and staged-generation kernel implementation**.

Implementation is active only under `include/mq/kernel`, `src/kernel`,
`apps/kernel`, and `tests/kernel`.

Audio synthesis is a separate future subsystem under `synthesis/`. It is not a
kernel or theory-language completion requirement.

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
22. exact `Same`, `Rise`, and `Fall` order certificates;
23. event-bounded phrase spans with exact cadence evidence and strength;
24. typed phrase function, cadence, and boundary pitch projections;
25. exact phrase-conditioned structural pitch requests;
26. one-event generation stages composed into multi-stage candidates;
27. seeded selection only after every candidate reaches an exact plan or
    retained rejection;
28. effective-profile differences exercised through generated outcomes;
29. typed grammar `Stage` and `Candidate` output forms;
30. complete grammar-to-staged-plan generation with scope-controlled output;
31. seed-driven neutral CLI generation through the same tested path;
32. validated jins descriptor catalog with tonic, ghammaz, register, emphasis,
    baggage, and ordered gesture capabilities;
33. descriptor-gated entry, evidence, event placement, and exact register or
    baggage evidence;
34. ordered gesture progress, completed event spans, and explicit gesture
    scope restoration;
35. a five-stage neutral descriptor and sayr witness with exact calculated
    root, `4:3` ghammaz, `3:2` baggage, `4:3` return, and root resolution
    targets;
36. executable profile narrowing that removes that travel candidate at its
    baggage step while retaining the diagnostic;
37. typed sayr plans, dependency-ordered obligations, proof-bearing
    fulfillment, route completion, and scope restoration;
38. thirty-seven registered executable law suites.

Checkpoint validation state:

- the preceding 34-suite descriptor checkpoint passed debug, optimized, and
  undefined-behavior-sanitized builds;
- the current sayr checkpoint completed a clean remote GCC 14 debug build,
  linked, and passed all 37 registered suite entry points;
- the first Colab attempt failed before project compilation because its
  default libstdc++ lacked C++23 `<expected>`;
- `tools/notebook/Build.ipynb` now installs and probes GCC 14, forces that
  compiler for every target, and starts from a clean clone;
- optimized, sanitizer, and CLI validation of the current checkpoint remain
  unverified. A later Modal attempt inherited ignored desktop build objects,
  so its partial results are deliberately not counted.

No real regional profile is claimed yet. The exact boundary is recorded in
`theory/implementation/kernel.md`.

The generator currently executes finite typed candidates governed by a small
neutral sayr obligation graph. It is not yet a recursive phrase, motif, timing,
or ornament generator, and the sayr vocabulary is not culturally complete.
Descriptor content is still neutral fixture data; provenance-bearing regional
descriptor reconstruction and sourced regional profiles remain unimplemented.

The normative model remains under `theory/`.
