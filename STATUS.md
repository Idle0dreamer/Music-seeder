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
3. declarative profile reconstruction and conflict detection;
4. evidence-gated tonal operators;
5. stable seeded lexicographic choice;
6. typed finite grammar terms and complete relational evaluation;
7. lexical binding and explicit scope restoration;
8. path history with source, target, level, and prerequisite guards;
9. executable law tests.

No real regional profile is claimed yet. The exact boundary is recorded in
`theory/implementation/kernel.md`.

The normative model remains under `theory/`.
