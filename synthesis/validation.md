# Synthesis validation

Synthesis validation is independent of musical-language validation. Passing
grammar laws or practitioner review does not validate audio, and passing
acoustic tests does not validate maqam behavior.

## Current engineering evidence

- Local `make synthesis` and the independent renderer produce a non-silent,
  bounded 48 kHz mono PCM WAV for Bayati seed `17`.
- The current render is 140640 frames, peak `0.92`, SHA-256
  `174241947fcf975e3867cfc6897e37f6c889daa058952f32f3ee482a3df72829`.
- A repeated local render is byte-identical.
- GitHub Actions run `30755938899` at exact head
  `985464504bf3f732a9ab3cbb5ce3416fa4b6bffb` passed the Faust probe and
  synthesis-render jobs and uploaded artifact `8836021086`.
- Local Faust compilation was not independently rerun because this host has
  no `cmake` executable; the pinned compiler generation and runtime path are
  validated by the remote job above.

These facts establish a reproducible software path, not santur authenticity.
Measured geometry, bridge/radiation references, overlap tolerances, and
perceptual review remain required.

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
