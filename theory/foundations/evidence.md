# Evidence discipline

## Claim classes

Every catalog entry and production rule carries one of these statuses:

| Status | Meaning |
| --- | --- |
| Attested | directly supported by an identified recording, transcription, or practitioner source |
| Corroborated | independently supported by multiple sources or performers |
| Profile-specific | valid only for a named region, repertoire, period, or performer |
| Analytical hypothesis | useful interpretation, not a settled practitioner fact |
| Provisional prior | numeric starting point awaiting corpus calibration |
| Unknown | deliberately unimplemented rather than guessed |

“Common,” “rare,” “characteristic,” and “forbidden” require citations or corpus
counts. They are not decorative adjectives.

## Evidence object

Any musical fact must retain:

```text
claim
scope
status
source
source location
recording interval, when applicable
annotator
confidence
counterexamples
revision history
```

The source location may be a page, measure, phrase interval, or recording
timestamp. A source title without a localizable example is insufficient for a
high-confidence gesture or cadence rule.

## Negative evidence

Absence from a small corpus does not prove a forbidden transition. The catalog
distinguishes:

- structurally impossible under the declared grammar;
- rejected by named practitioners;
- unattested in the current corpus;
- uncommon;
- unknown.

Only the first two can create a hard prohibition.

## Disagreement

Conflicting analyses remain multiple scoped interpretations. The system must
not average them into a false consensus.

Examples include:

- different jins sizes under different analytical conventions;
- whether a passage is color, internal jins movement, or maqām-level
  modulation;
- whether a pitch difference is structural, regional, or individual;
- different Bayati/Husayni classifications.

## Evidence-to-rule promotion

A candidate rule becomes generative only after:

1. its scope is explicit;
2. at least one positive example is localized;
3. counterexamples have been considered;
4. its effect can be represented by existing primitives;
5. it has a validation test;
6. its confidence is not overstated.

Numeric parameters additionally require a sample count and uncertainty.

## No invented completion

Missing vocabulary remains missing. A generator must prefer a shorter,
well-supported performance over filling an unknown region with generic motion.
