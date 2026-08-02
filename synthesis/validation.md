# Synthesis validation

Synthesis validation is independent of musical-language validation. Passing
grammar laws or practitioner review does not validate audio, and passing
acoustic tests does not validate maqam behavior.

## Adapter contract

- Unsupported execution intents refuse or produce an explicit reviewed
  mapping.
- Event, timing, pitch-trajectory, and seed identities survive translation.
- Numerical conversion declares tolerances and never feeds approximations back
  into grammar legality.

## Isolated sound

- Attack, spectral trajectory, decay, beating, and dynamic response match
  adapter-specific reference distributions.
- No fixed narrow resonance dominates unrelated pitches.
- A continuous frequency trajectory remains continuous through the declared
  numerical conversion boundary; no hidden note quantization is introduced.
- Hammer, bow, breath, or reed excitation is tested separately from resonator
  response so an envelope cannot masquerade as an instrument model.

## Overlap

- New attacks remain identifiable after preceding sounds.
- Repeated-event and fast-cell masking remain within reference tolerance.
- Persistent state, damping, energy, and limiting do not erase later events.
- Course coupling, sympathetic resonance, and bridge/soundboard energy transfer
  remain measurable under repeated and overlapping attacks.

## Perception

Instrument plausibility is rated separately from maqam, jins, phrasing,
cadence, intonation, and regional fit.

No adapter is released without its own coverage matrix, references, numerical
tolerances, known limitations, and isolated/overlap evidence.
