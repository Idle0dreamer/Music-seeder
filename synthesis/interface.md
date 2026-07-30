# Performance-plan boundary

The theory and language kernel ends at an instrument-neutral performance plan.
The synthesis subsystem begins by translating that plan into one
instrument-specific execution.

## Kernel output

The current kernel carries stable structural events and exact structural pitch
targets in an instrument-neutral prefix plan. The complete future contract also
carries:

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
