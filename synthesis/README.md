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
- `physical-modeling.md` defines the actual-sound objective and the candidate
  physical-modeling foundation.
- `models/` contains small build-time Faust probes. They are acoustic seams,
  not named-instrument or regional-authenticity claims.
- `../src/synthesis/Render.cpp`, `../src/synthesis/santur/`, and
  `../apps/synthesis/` contain the first provisional coupled-course santur
  renderer and WAV boundary.
- `../src/synthesis/FaustRender.cpp` and the `synthesis-faust` Make target
  consume generated Faust C++ through the same boundary.
- `../third_party/audio/` records pinned Git source checkouts for the future
  synthesis adapter; they are not linked into the kernel.
- `instruments/` contains one directory per instrument model.
- `validation.md` owns acoustic and renderer validation.
- `sources.md` owns synthesis-specific engineering sources.
- `debt.md` owns unresolved synthesis research.

The first provisional santur adapter is implemented and renders a deterministic
WAV. It includes three detuned course strings, stiff-string partials, a
hammer-contact attack, bridge and soundboard modes, and sympathetic tails. The
Faust-generated coupled-course renderer is implemented as a separate target;
its clean remote build and artifact are still pending. Neither is yet a
validated instrument reproduction: geometry, material parameters, measurements,
and perceptual comparison remain open.

Future code belongs under independent `include/mq/synthesis`,
`src/synthesis`, and `tests/synthesis` trees. It does not belong under
`mq/kernel`.
