# Project status

The previous generator executable and audio output are rejected exploratory
prototypes. They are not included in this checkpoint and are not evidence of a
completed maqam generator or audio renderer.

Current phase: **non-ML operator-algebra kernel implementation**.

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
9. declarative profile reconstruction and conflict detection;
10. evidence-gated tonal operators;
11. stable seeded lexicographic choice;
12. typed finite grammar terms and complete relational evaluation;
13. lexical binding and explicit scope restoration;
14. path history with source, target, level, and prerequisite guards;
15. sixteen executable law suites.

No real regional profile is claimed yet. The exact boundary is recorded in
`theory/implementation/kernel.md`.

The normative model remains under `theory/`.
