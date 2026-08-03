# Music-seeder

Music-seeder is a non-ML, mathematically explicit generative music system. Its
current theory module addresses maqām performance: typed musical operators,
exact composition laws, and declarative, provenance-bearing reconstruction of
regional, school, repertoire, performer, and form variants.

Maqām is the present research scope, not the identity or eventual limit of the
product. Other theory modules may be added later through explicit boundaries;
none are modeled or claimed by the current kernel.

The old seed-driven generator and synthesized audio prototypes remain rejected
diagnostic artifacts. The current C++23 Ajam, Bayati, Hijaz, Kurd, Nahawand,
Nikriz, and Rast packages are loaded from provenance-bearing collection records
and built through shared family machinery. Rast, Nahawand, Hijaz, Kurd, Ajam,
and Nikriz are admitted as complete canonical packages; Bayati is executable
but remains explicitly incomplete until its profile-specific completion gates
pass. The normative theory for the current maqām module is
under `theory/`; the implementation is isolated under
`include/mq/kernel`, `src/kernel`, `apps/kernel`, and `tests/kernel`.

Pitch is calculated from symbolic ratio and contextual constraints. No scale
array, 24-TET table, neural network, gradient descent, or backpropagation is the
grammar authority.

## Build and run

Binary builds and executable validation run in the free GitHub Actions workflow
`.github/workflows/kernel.yml`. The workflow installs the pinned toolchain,
builds the kernel and synthesis targets, generates the Faust C++ sources, runs
the remote CLI/tests, and uploads the executables and rendered WAV artifacts.
The constrained development machine does not compile these targets. Download
the artifacts from the green run, then run the downloaded binaries locally;
`output/remote-binaries-current` is the local runtime alias used by the desktop
shortcut.

```sh
# On GitHub Actions, the workflow runs these recipes:
make kernel
make kernel-test

# After downloading the matching green-run artifacts:
./output/remote-binaries-current/kernel/kernel 2
./output/remote-binaries-current/kernel/kernel-tests
./output/remote-binaries-current/player/synthesis-render \
  --maqam sikah --seed 53 --output output/sikah.wav
./output/remote-binaries-current/player/synthesis-ui
```

The optional unsigned seed drives the neutral executable generation fixture.
The Ajam, Bayati, Hijaz, Kurd, Nahawand, Nikriz, and Rast routes are available
as `./output/remote-binaries-current/kernel/kernel ajam [seed]`,
`./output/remote-binaries-current/kernel/kernel bayati [seed]`,
`./output/remote-binaries-current/kernel/kernel hijaz [seed]`,
`./output/remote-binaries-current/kernel/kernel kurd [seed]`,
`./output/remote-binaries-current/kernel/kernel nahawand [seed]`,
`./output/remote-binaries-current/kernel/kernel nikriz [seed]`, and
`./output/remote-binaries-current/kernel/kernel rast [seed]`. They print complete candidates and exact timed
structural targets; the kernel is not itself an audio renderer.

`make test` is an alias for the kernel law suite. It is a remote CI recipe in
this constrained checkout; local execution uses the downloaded artifact
instead.

## Source map

| Area | Responsibility |
| --- | --- |
| `theory/algebra` | sorts, operators, semantics, laws, regional reconstruction |
| `theory/pitch` | exact calculated pitch-field specification |
| `include/mq/kernel/grammar` | finite terms, guards, scope policy, derivations |
| `include/mq/kernel/generate` | staged complete-candidate evaluation and seeded selection |
| `include/mq/kernel/state` | decomposed musical-state projections |
| `include/mq/kernel/performance` | instrument-neutral structural events and exact targets |
| `include/mq/kernel/motion` | typed intended melodic direction |
| `include/mq/kernel/path` | constrained modulation graph rules |
| `include/mq/kernel/pitch` | exact fields, requests, expressions, and constraint systems |
| `include/mq/kernel/profile` | rules, patches, and effective profiles |
| `src/kernel/*` | implementations separated by the same domains |
| `tests/kernel` | executable algebra and reconstruction laws |
| `theory/data/maqam` | collection records for executable package reconstruction |
| `synthesis` | separate audio interface, instrument adapters, and acoustic validation |

## Scope

The declared Arabic repertoire boundary is in
[`theory/maqamat/scope.md`](theory/maqamat/scope.md). Rast is currently the
first complete canonical package; Nahawand, Hijaz, Kurd, Ajam, and Nikriz are
also admitted as complete canonical packages. Bayati is executable but remains
incomplete under its case-study gates. Remaining entries are catalogued and refused until
their own positive and negative rules, formulas, vocabulary, pathways, and
provenance evidence are implemented.

## Research basis

- Sami Abu Shumays, [“Maqam Analysis: A Primer”](https://www.maqamlessons.com/analysis/media/MaqamAnalysisAPrimer_2013WebFormat.pdf),
  *Music Theory Spectrum* 35(2), 2013. This is the main basis for
  tonicization-centered ajnas, jins baggage,
  the three strengths of modulatory motion, constrained jins networks, and the
  regional scope.
- Johnny Farraj and Sami Abu Shumays, [MaqamWorld](https://www.maqamworld.com/en/maqam.php),
  especially the general maqam, Bayati, Bayati-jins, iqa, and form materials.
See `theory/README.md` and `theory/implementation/kernel.md` for the model and
the exact implementation boundary. Audio work has a separate authority under
`synthesis/`.

## Licensing

Code is MPL-2.0. The theory, current synthesis documentation, and other written
work are CC BY-SA 4.0. Profiles require explicit provenance and rights review;
the project licenses cannot override source, repertoire, or community
restrictions. See `LICENSE_POLICY.md`, `ATTRIBUTION.md`, and `REUSE.toml`.
