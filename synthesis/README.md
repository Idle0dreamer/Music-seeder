# Audio synthesis

Audio synthesis is a separate subsystem from the maqam theory and language
kernel.

The dependency direction is one-way:

```text
theory and language kernel
        |
        v
instrument-neutral performance plan
        |
        v
synthesis adapter
        |
        v
audio
```

The kernel must not depend on a waveform engine, instrument geometry, resonator
state, sample rate, or audio library. A synthesis adapter may depend on the
kernel's exported performance-plan contract.

## Structure

- `interface.md` defines the future boundary between a performance plan and an
  audio adapter.
- `instruments/` contains one directory per instrument model.
- `validation.md` owns acoustic and renderer validation.
- `sources.md` owns synthesis-specific engineering sources.
- `debt.md` owns unresolved synthesis research.

Only the hammered dulcimer adapter is currently specified. It is not
implemented, and other instruments are not being designed now.

Future code belongs under independent `include/mq/synthesis`,
`src/synthesis`, and `tests/synthesis` trees. It does not belong under
`mq/kernel`.
