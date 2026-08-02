# Synthesis research debt

The provisional coupled-course santur adapter and WAV boundary are implemented.
The unresolved validation and instrument-identification work is:

- exact instrument family and playing technique to emulate;
- geometry and material measurements;
- hammer contact parameters and strike-position behavior;
- bridge mobility and soundboard radiation measurements;
- reference decay and masking distributions by pitch and dynamic;
- perceptual definition of crispness for qualified listeners;
- performance-intent mapping and refusal rules at the adapter boundary.
- a continuous-frequency input contract that does not make MIDI, 12-TET, or a
  static tuning table authoritative;
- a choice between Faust-generated C++ models, independently implemented C++
  models, or a carefully isolated combination;
- nonlinear hammer/string contact, stiff-string dispersion, course detuning,
  bridge impedance, soundboard modes, sympathetic coupling, and
  strike-position-dependent radiation;
- license and patent review for every reused model, rawwave, or generated
  asset;
- isolated and overlap tests that catch a single resonant note masking later
  events.

The current model is not accepted as an authentic named-instrument result. It
proves that exact kernel targets cross the numerical conversion boundary and
exercise an explicit coupled physical approximation.

No generic instrument fallback may fill these gaps.
