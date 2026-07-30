# Validation gates

Passing software tests is necessary but not musical validation. Gates are
ordered; failure freezes downstream claims.

## G0 — evidence and scope

- Every active profile claim has a source and evidence status.
- Unsupported regions, maqamat, or forms refuse generation.
- Conflicting sources remain visible.

## G1 — representation

- Experts can annotate the proposed primitives without systematic forced fits.
- Inter-annotator agreement and calibrated disagreement are reported by layer.
- The schema can represent alternative tonicization analyses.

## G2 — hard structural legality

- Generated graph edges and path prerequisites are legal.
- Tonicization claims meet evidence and residence thresholds.
- Cadences, return paths, ornaments, and articulation intents satisfy preconditions.
- Property-based tests search for counterexamples over many seeds.

## G3 — corpus fit

- Held-out distributions match phrase lengths, motif recurrence, transformations,
  cadence types, register paths, modulation levels, and timing.
- Metrics are stratified by profile and performer.
- Likelihood alone is not accepted if output collapses to short common patterns.

## G4 — intonation

- Conditional pitch and trajectory distributions are calibrated on held-out
  performances.
- Direction, neighbor, cadence, and performer effects are recovered where data
  supports them.
- Experts conduct context-bearing listening tests, not isolated-cent judgments
  alone.

## G5 — musical perception

Blinded practitioners rate:

- maqam and jins identity;
- idiomatic phrase behavior;
- modulation interpretation;
- cadence and return;
- regional/repertoire fit;
- intonation;
- coherence across multiple phrases.

Human and generated examples are loudness-matched and randomized. The system
reports confidence intervals and systematic failure descriptions.

## G6 — diversity and provenance

- Seeds yield meaningful structural alternatives without illegal novelty.
- Motif recurrence remains coherent.
- Nearest-neighbor checks guard against memorizing recordings.
- Attribution and rights requirements are satisfied.

## G7 — coverage release

A profile is released only with a published coverage matrix, known limitations,
test corpus description, and gate results. “Full-depth maqam generation” is not
a project-wide boolean; it is a claim scoped to a named, versioned profile.

Acoustic and instrument plausibility gates belong to
[`synthesis/validation.md`](../../synthesis/validation.md), not this sequence.
