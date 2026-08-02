# Physical sound foundation

This subsystem is for the actual sound itself. A frequency trajectory, an
envelope, or a resonant filter is not an instrument model. The target is the
audible result of excitation, coupled energy, dispersion, resonance, damping,
and radiation, with enough evidence to explain why the result sounds like the
claimed instrument.

This work remains downstream of the instrument-neutral performance plan. Faust,
STK, or any other audio technology must not become a dependency of
`mq/kernel`.

## Foundation direction

The strongest initial foundation is the [Faust Physical Modeling
Library](https://faustlibraries.grame.fr/libs/physmodels/) compiled to C++.
Faust is a build-time mathematical specification language here, not a runtime
or scripting requirement. Its compiler produces a normal C++ DSP class, and
the generated runtime can remain entirely C++.

Use the following projects according to their role:

| Project | Role | Boundary or catch |
| --- | --- | --- |
| [Faust Physical Modeling Library](https://faustlibraries.grame.fr/libs/physmodels/) | Waveguides, mass-spring systems, strings, bridges, wind elements, reeds, modal percussion, exciters, and vocal components | Models are authored in Faust and compiled into self-contained C++ |
| [mesh2faust](https://github.com/grame-cncm/faust/blob/master-dev/tools/physicalModeling/mesh2faust/README.md) | Finite-element modal precomputation from 3D geometry and material parameters | Useful for bars, plates, bodies, and imaginary instruments; precomputation can be expensive |
| [STK](https://github.com/thestk/stk) | Permissively licensed C++ reference implementations for waveguides, modal instruments, strings, winds, and percussion | Reference designs only until license, rawwave, and patent notes are reviewed |
| [Q DSP](https://github.com/cycfi/q) | General C++ DSP support such as filters and signal utilities | Support library, not an instrument-model collection |
| [Surge Tuning Library](https://github.com/surge-synthesizer/tuning-library) | Interoperability with `.scl` and `.kbm` tuning data | Static tuning tables cannot represent contextual maqam intonation by themselves |
| [RipplerX](https://github.com/tiagolr/ripplerx) | Reference for coupled modal resonators and struck/plucked sounds | GPL-3.0; reference unless the combined project adopts compatible licensing |
| [Resonarium](https://github.com/gabrielsoule/resonarium) | Reference for coupled string waveguides and expressive excitation | GPL; development, CPU, and stability limitations remain |

The first implementation should be a small C++ adapter around generated or
independently implemented models. These projects provide acoustic machinery;
they do not provide a faithful model of every named world instrument.

## Continuous pitch authority

The physical layer must not receive `C-sharp`, a MIDI note number, or a hidden
12-TET assumption as its fundamental authority. It receives a continuous
trajectory:

```text
f(t) = f_tonic * R(jins, direction, phrase, region, performer, t) * 2^k
```

`R` is a contextual frequency ratio. It may change during a note with ascent or
descent, current jins, modulation, attraction, ornament, and performer
practice. An adapter may accept `fundamental_hz`, a ratio trajectory, or both,
provided the conversion boundary and interpolation behavior are explicit.

MIDI conversion, semitone-quantized controls, fixed pitch-bend ranges, and
factory presets calibrated only at 12-TET are optional integrations, never
theory or synthesis authorities.

## Physical target

The conceptual model is a coupled physical system:

```text
M q''(t) + C q'(t) + K q(t) = B F_excitation(t)
y(t) = L q(t)
```

`M`, `C`, and `K` describe mass, damping, and stiffness; `F_excitation` is a
pluck, hammer, bow, breath, or reed force; and `y(t)` is the radiated signal.
Waveguides and modal reductions are valid numerical realizations when their
approximations and limits are documented. Nothing in this model requires a
Western tuning system.

## First instrument: hammered dulcimer / santur family

A generic modal tone is not enough. The adapter must account for:

- nonlinear hammer-string collision;
- multiple strings per course and their small detuning;
- stiff-string dispersion;
- bridge impedance and energy transfer;
- soundboard modes and radiation;
- sympathetic coupling;
- strike-position-dependent modal amplitudes; and
- the hard transient and rapid radiation that produce crisp attacks.

The earlier failure mode where one note overtakes everything is evidence of an
under-modeled resonant tone, not evidence of an instrument. Attack spectrum,
beating, decay, masking, overlap, and dynamic response must be measured and
validated independently.

`mesh2faust` is a promising precomputation route for bodies, bridges, plates,
and soundboards because it can derive modal systems from geometry and material
properties. It is not a substitute for hammer contact, string coupling, or
instrument-specific validation.

## Evidence boundary

Sound can be generated without a corpus. Recordings and physical measurements
are required when claiming authenticity or a particular instrument family:
bridge response, modal decay, attack spectrum, bore or cavity impedance where
applicable, coupling, and performer-dependent behavior.

That is system identification and validation, not machine learning. The
project must keep the physical model explicit, inspectable, and replaceable.
No generic instrument fallback may fill missing regional or instrument
knowledge.

The C++ provisional coupled-course santur adapter now renders a WAV without
entering `mq/kernel`. The pinned Faust workflow generates and compiles the
corresponding physical-modeling probe online. The C++ model is an explicit
engineering approximation, not a validated named-instrument implementation.
Faust, Faust Libraries, STK, Q, and Surge Tuning remain Git-pinned external
inputs under `third_party/audio/`; none is linked into the kernel.
