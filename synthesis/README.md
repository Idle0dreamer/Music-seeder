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
  `../apps/synthesis/` contain the coupled-course santur renderer and WAV
  boundary. Native event coefficients and strike geometry are prepared once
  per event; the renderer mixes only each voice's declared audible tail.
- `../src/kernel/maqam/collection/` owns the data-driven package catalog used
  by both renderers. Adding a collection-defined family does not require a
  renderer branch.
- The Qt player launches the renderer asynchronously, supports configurable
  long performance durations, collection-defined route continuations, and an
  explicit optional playback loop. The generated file itself is one rendered
  performance stream; it is not assembled by copying one audio clip. The
  player displays the rendered WAV and a soft Fourier magnitude view.
- `../src/synthesis/FaustRender.cpp` and the `synthesis-faust` Make target
  consume generated Faust C++ through the same boundary.
- `../third_party/audio/` records pinned Git source checkouts for the future
  synthesis adapter; they are not linked into the kernel.
- `instruments/` contains one directory per instrument model.
- `validation.md` owns acoustic and renderer validation.
- `sources.md` owns synthesis-specific engineering sources.
- `debt.md` owns unresolved synthesis research.

The current santur adapter renders a deterministic WAV. It includes three
detuned course strings, stiff-string partials, a hammer-contact attack, bridge
and soundboard modes, and sympathetic tails. The registered maqam generator
supplies exact timing, intensity, articulation, ornament, and release intent
to this adapter. The Faust-generated coupled-course renderer is a separate
compiled C++ DSP target. Faust retains resonator state between audio blocks;
it is an executable instrument, not a pre-rendered sample bank. Exact head
`cb0cf03d1359d5efe446b0ed6be6b8bd84d73b9b` passed remote build and synthesis
run `30803900916`. The downloaded native and Faust binaries rendered the same
collection-driven Bayati plan locally for 31.9887 seconds; the local Faust WAV
was byte-identical to the remote artifact. This proves the execution boundary,
not an authentic santur reproduction or a complete Bayati realization.
Neither is yet a validated instrument reproduction: geometry, material
parameters, measurements, and perceptual comparison remain open.

Future code belongs under independent `include/mq/synthesis`,
`src/synthesis`, and `tests/synthesis` trees. It does not belong under
`mq/kernel`.
