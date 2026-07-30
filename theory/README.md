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
12. [Performance state](grammar/state.md)
13. [Motif grammar](grammar/motif.md)
14. [Phrase and cadence grammar](grammar/phrase.md)
15. [Sayr](grammar/sayr.md)
16. [Modulation and tonicization](grammar/modulation.md)
17. [Contextual intonation](performance/intonation.md)
18. [Free and measured time](performance/time.md)
19. [Ornament and articulation](performance/ornament.md)
20. [Form separation](form/form.md)
21. [Regional and repertoire profiles](region/profile.md)
22. [Hammered-dulcimer realization](instrument/dulcimer.md)
23. [Unified constraint model](math/model.md)
24. [Corpus and annotation model](data/corpus.md)
25. [Validation gates](validation/gates.md)
26. [Bayati case study](case/bayati.md)
27. [Research debt](validation/debt.md)
28. [Kernel implementation ledger](implementation/kernel.md)
29. [Sources](sources/bibliography.md)

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
+ instrument realization model
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
- the hammered-dulcimer model passes isolated-note decay and masking tests.

Passing software tests alone is insufficient.

## Non-ML boundary

The grammar is not inferred by backpropagation. It is an explicit typed
operator algebra with declarative profile rules, exact constraint composition,
conflict detection, and executable laws. Recordings and expert analysis may
supply reviewed constants or demonstrate that a rule is attested; they do not
replace the grammar with an opaque predictor.
