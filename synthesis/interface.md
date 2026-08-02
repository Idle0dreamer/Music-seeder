# Performance-plan boundary

The theory and language kernel ends at an instrument-neutral performance plan.
The synthesis subsystem begins by translating that plan into one
instrument-specific execution.

## Kernel output

The current kernel carries stable structural events and exact structural pitch
targets in `performance::Plan::events`. Each current event also carries an
exact rational onset and duration, intensity, articulation intent, and typed
monophonic strand. This is the first executable timing seam; the complete
future contract also carries:

- stable event and gesture identities;
- structural roles and phrase boundaries;
- score time and performed-time trajectories;
- exact pitch trajectories until the declared frequency conversion boundary;
- dynamics, emphasis, connection, articulation, and release intents;
- dependencies between simultaneous and successive events;
- a deterministic synthesis seed partition separate from grammar choice.

It does not carry courses, string geometry, hammer contact constants,
soundboard modes, sample rate, microphone geometry, or persistent resonator
state.

## Frequency input

The adapter receives continuous frequency authority from the performance plan:

```text
f(t) = f_tonic * contextual_ratio(t) * 2^k
```

`contextual_ratio(t)` may depend on jins, direction, phrase, region, performer,
modulation, attraction, and ornament. MIDI notes, 12-TET tables, and fixed
pitch-bend ranges are not valid substitutes for this contract. Numerical
conversion is owned by the adapter at its declared boundary and must not feed
approximations back into grammar legality.

## Adapter responsibility

An instrument adapter:

1. declares which performance intents it can realize;
2. rejects an unsupported plan or returns an explicit mapping trace;
3. maps supported intents to instrument actions;
4. owns persistent acoustic state and overlap;
5. converts exact trajectories at a documented numerical boundary;
6. renders audio within published error and resource limits.

The adapter may alter physical execution and audibility. It may not rewrite
tonal role, phrase function, modulation, cadence, or regional theory.

The current `synthesis-render` target implements this boundary for a provisional
coupled-course santur model and writes mono PCM WAV. The separate
`synthesis-faust-render` target drives generated Faust C++ through the same
boundary and refuses unsupported articulation. Neither is yet a validated
hammered-dulcimer reproduction.
