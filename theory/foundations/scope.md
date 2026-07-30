# Scope and non-goals

## Initial cultural scope

The first model is limited to twentieth- and twenty-first-century urban Arabic
maqām practice in Egyptian and Levantine repertoires, with taqsīm as the first
form and Maqām Bayati as the first case study.

This scope does not silently include:

- Iraqi maqām suites;
- Gulf ṣawt or related practices;
- Ottoman/Turkish makam;
- Persian dastgāh;
- Byzantine echoi;
- North African Andalusian nūba traditions;
- popular keyboard approximations;
- pedagogical 24-tone equal-tempered abstractions.

Related traditions may share structures, terminology, or historical material.
They remain separate profiles until comparative evidence supports a shared
rule.

## What “correct” means

Correctness has three distinct meanings:

1. **Representational correctness**  
   The primitives can describe what practitioners identify without forcing it
   into a scale-only model.
2. **Generative correctness**  
   Produced phrases follow attested vocabulary, pathway, cadence, timing, and
   intonation behavior.
3. **Perceptual correctness**  
   acculturated listeners hear the intended jins, maqām, phrase function, and
   degree of closure.
These must be tested separately. Acoustic correctness belongs to the separate
synthesis subsystem.

## Non-goals

The theory does not claim:

- a canonical list of every jins or maqām;
- one “correct” cents value for a degree;
- that rational ratios generate Arabic intonation;
- that every legal path is equally common;
- that a modulation is proved by an accidental;
- that taqsīm is random or temporally structureless;
- that a universal Middle Eastern profile is valid;
- that a Western ornament name defines Arabic execution;
- that automated metrics can replace practitioner judgment.

## Required separation of concerns

The following layers must not be collapsed:

| Layer | Describes |
| --- | --- |
| Tonal relation | relative pitch identity and function |
| Jins | tonicized melodic unit and its vocabulary |
| Maqām | pathway field, root identity, and developmental expectations |
| Phrase | rhetorical and cadential unit |
| Form | taqsīm, mawwāl, composition, song section, and so forth |
| Iqāʿ | independently selected rhythmic cycle |
| Region/repertoire | historically and socially bounded practice |
| Performer | reviewed vocabulary, habits, range, and intonation |
| Performance plan | instrument-neutral pitch, time, dynamic, and gesture intent |

An implementation may connect these layers through typed interfaces. It may not
pretend one layer is an intrinsic property of another.

The language ends at the performance plan. Instrument action and waveform state
belong to the separate synthesis adapter boundary.
