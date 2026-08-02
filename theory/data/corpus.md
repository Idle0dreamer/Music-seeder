# Corpus and annotation model

The theory becomes executable only through sourced, profile-matched data.

## Entities

The corpus stores separate entities for:

- source and rights;
- recording and signal provenance;
- performance;
- performer, ensemble, instrument, and tuning context;
- tradition, region, repertoire, and era;
- section and phrase;
- cell occurrence and motif family;
- structural pitch event and continuous pitch trajectory;
- jins and maqam interpretation;
- tonicization episode and modulation path;
- cadence;
- timing and boundary;
- ornament and articulation;
- annotator, confidence, and disagreement.

Every derived claim links back to the exact evidence segments that support it.

## Annotation layers

1. **Signal** — attacks, offsets, pitch tracks, loudness, spectral descriptors.
2. **Structural events** — host tones, pitch roles, ornaments, grouping.
3. **Vocabulary** — cells, motif identity, transformations.
4. **Tonal interpretation** — jins, tonic, level, evidence, competing analyses.
5. **Phrase syntax** — function, relations, cadence, boundary strength.
6. **Sayr and form** — landmarks, obligations, sections, climax and return.
7. **Performance** — intonation, timing, articulation, and observed execution.

No automated layer is promoted to ground truth without expert review.

## Executable casebook boundary

The first native boundary is
[`theory/data/corpus/amicor-timing.casebook`](../data/corpus/amicor-timing.casebook).
`validation::Casebook` preserves source, location, rights, scope, annotator,
confidence, disagreement, performer, recording, split, exact relative event
durations, releases, ornament fractions, and pauses. Its report computes exact
means and refuses the complete timing gate when profile, split, performer,
release, ornament, or pause coverage is absent.

The fixture is keyed to the public AMICOR symbolic corpus and deliberately
marks its values as test data rather than source measurements. It exercises the
ingestion and validation seam; it is not a substitute for the profile-matched
Bayati audio/casebook needed for a full-depth regional claim.

## Disagreement

Annotations are distributions:

\[
P(a\mid x)=\sum_j P(a\mid x,j)P(j),
\]

where annotator reliability can vary by layer and repertoire. Competing plausible
analyses are retained. Consensus is not manufactured by deleting minority
interpretations.

## Sampling

Corpus design balances:

- maqam and jins context;
- performer and school;
- region and era;
- form and rhythmic context;
- instrument;
- phrase and cadence function;
- recording quality.

One celebrated performer or pedagogical example cannot stand in for a regional
practice.

## Splits

Train, development, and test partitions are separated by complete performance.
Stronger tests hold out performers, recordings, and repertoire families.
Motif-near-duplicates are clustered before splitting.

## Data-derived versus encoded

The system calculates:

- conditional pitch distributions;
- graph edge weights and duration models;
- motif transformations and recognition thresholds;
- phrase production weights;
- timing and ornament distributions.

Experts encode or verify:

- interpretive labels;
- hard exclusions and prerequisites;
- profile boundaries;
- source reliability;
- whether a discovered pattern is idiomatic or incidental.
