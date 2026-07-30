# Project status

The previous generator executable and audio output are rejected exploratory
prototypes. They are not included in this checkpoint and are not evidence of a
completed maqam generator or hammered dulcimer.

Current phase: **non-ML operator-algebra kernel implementation**.

Implementation is active only under `include/mq/kernel`, `src/kernel`,
`apps/kernel`, and `tests/kernel`.

Implemented so far:

1. exact rational arithmetic;
2. symbolic prime-factor pitch expressions and exact affine solving;
3. proof-producing exact order over symbolic pitches;
4. exact hard affine inequality feasibility;
5. exact witnesses for underdetermined hard pitch regions;
6. stable hard-constraint identities and contradiction traces;
7. declarative profile reconstruction and conflict detection;
8. evidence-gated tonal operators;
9. stable seeded lexicographic choice;
10. typed finite grammar terms and complete relational evaluation;
11. lexical binding and explicit scope restoration;
12. path history with source, target, level, and prerequisite guards;
13. twelve executable law suites.

No real regional profile is claimed yet. The exact boundary is recorded in
`theory/implementation/kernel.md`.

The normative model remains under `theory/`.
