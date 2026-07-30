# Maqām generative theory

This directory is the normative specification for any future generator. The
existing executable is a rejected prototype and is not evidence that this
theory has been implemented.

The model is intentionally modular. No single document is allowed to stand in
for the whole tradition, and no scale table is allowed to stand in for a
maqām.

## Reading order

1. [Scope and non-goals](foundations/scope.md)
2. [Evidence discipline](foundations/evidence.md)
3. [Pitch and tonal functions](primitives/pitch.md)
4. [Jins](primitives/jins.md)
5. [Hierarchical musical units](primitives/units.md)
6. [Sorts and identities](algebra/sorts.md)
7. [Primitive operators](algebra/operators.md)
8. [Operational semantics](algebra/semantics.md)
9. [Algebraic laws](algebra/laws.md)
10. [Regional reconstruction](algebra/reconstruct.md)
11. [Calculated pitch fields](pitch/solve.md)
12. [Exact pitch ordering](pitch/order.md)
13. [Hard pitch feasibility](pitch/feasibility.md)
14. [Exact contextual tendency tier](pitch/tendency.md)
15. [Lexicographic tendency sequence](pitch/sequence.md)
16. [Contextual pitch-field assembly](pitch/field.md)
17. [State projection into pitch context](pitch/project.md)
18. [Performance state](grammar/state.md)
19. [Motif grammar](grammar/motif.md)
20. [Phrase and cadence grammar](grammar/phrase.md)
21. [Sayr](grammar/sayr.md)
22. [Modulation and tonicization](grammar/modulation.md)
23. [Contextual intonation](performance/intonation.md)
24. [Free and measured time](performance/time.md)
25. [Ornament and articulation](performance/ornament.md)
26. [Form separation](form/form.md)
27. [Regional and repertoire profiles](region/profile.md)
28. [Unified constraint model](math/model.md)
29. [Corpus and annotation model](data/corpus.md)
30. [Validation gates](validation/gates.md)
31. [Bayati case study](case/bayati.md)
32. [Research debt](validation/debt.md)
33. [Kernel implementation ledger](implementation/kernel.md)
34. [Sources](sources/bibliography.md)

## Central conclusion

A defensible procedural system is not:

```text
maqam = scale + random notes + ornaments
```

It is:

```text
universal typed engine
+ tradition/repertoire profile
+ maqam-specific pathway model
+ reviewed melodic and ornament lexicons
+ hierarchical phrase/form grammar
+ calculated contextual intonation field
+ instrument-neutral performance plan
+ evidence and validation
```

The engine may be uniform. Its musical content cannot be uniform across
Egyptian, Levantine, Iraqi, Gulf, Turkish, North African, and other practices.

## Implementation hold

No production implementation should begin until:

- every primitive has an unambiguous identity and scope;
- the Bayati case study can be annotated consistently by practitioners;
- tonicization and cadence evidence can be measured over time;
- a small, licensed reference corpus exists;
- intonation is represented as conditional performance data rather than a
  tuning formula;
- the instrument-neutral performance-plan boundary is typed and testable.

Passing software tests alone is insufficient.

## Non-ML boundary

The grammar is not inferred by backpropagation. It is an explicit typed
operator algebra with declarative profile rules, exact constraint composition,
conflict detection, and executable laws. Recordings and expert analysis may
supply reviewed constants or demonstrate that a rule is attested; they do not
replace the grammar with an opaque predictor.

Audio synthesis is not part of this theory tree. It consumes a future
instrument-neutral performance plan through the boundary defined under
[`synthesis/`](../synthesis/README.md).
