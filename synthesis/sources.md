# Synthesis sources

These sources support the prospective hammered dulcimer adapter. They do not
support claims about maqam grammar or regional practice.

## Physical-modeling foundations

- [Faust Physical Modeling Library](https://faustlibraries.grame.fr/libs/physmodels/).
  Waveguide, mass-spring, string, bridge, wind, exciter, modal-percussion, and
  vocal physical-modeling components.
- [Faust compiler documentation](https://faustdoc.grame.fr/manual/compiler/).
  Build-time generation of C++ DSP code and optional vectorized code paths.
- [mesh2faust](https://github.com/grame-cncm/faust/blob/master-dev/tools/physicalModeling/mesh2faust/README.md).
  Finite-element modal generation from volumetric geometry, material
  parameters, excitation positions, and optional fundamental-frequency control.
- [STK](https://github.com/thestk/stk).
  Open-source C++ signal-processing and algorithmic-synthesis reference code.
  License, included rawwaves, and legal notes require review before reuse.
- [Q DSP](https://github.com/cycfi/q).
  C++ DSP primitives and signal utilities; not an instrument-model authority.
- [Surge Tuning Library](https://github.com/surge-synthesizer/tuning-library).
  `.scl`/`.kbm` parsing and frequency lookup for interoperability only.

The compatible source candidates are now recorded as pinned Git checkouts under
[`third_party/audio/`](../third_party/audio/README.md). They remain external
works and are not current kernel or production-runtime dependencies.
[RipplerX](https://github.com/tiagolr/ripplerx) and
[Resonarium](https://github.com/gabrielsoule/resonarium) are useful physical
modeling references, but their GPL licensing must be resolved before any code
is reused in this project.

- Rainer Reuter and Alexander Zietlow,
  [Investigation of the Acoustical Properties of a Hammered
  Dulcimer](https://pub.dega-akustik.de/DAGA_2010/data/articles/000215.pdf).
  Supports strike-position, angle, force, and velocity effects; coupled
  string-bridge-soundboard behavior; and decay as a quality parameter.
- Federico Fontana et al.,
  [Physics-Based Sound Synthesis of the Hammered
  Dulcimer](https://www.dafx.de/paper-archive/2023/DAFx23_paper_18.pdf).
  Supports nonlinear hammer/string contact, stiff-string modeling, variable
  strike position, and time-varying synthesis parameters.
