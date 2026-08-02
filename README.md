# Music-seeder

Music-seeder is a non-ML, mathematically explicit generative music system. Its
current theory module addresses maqām performance: typed musical operators,
exact composition laws, and declarative, provenance-bearing reconstruction of
regional, school, repertoire, performer, and form variants.

Maqām is the present research scope, not the identity or eventual limit of the
product. Other theory modules may be added later through explicit boundaries;
none are modeled or claimed by the current kernel.

The old seed-driven generator and synthesized audio prototypes remain rejected
diagnostic artifacts. The current C++23 Bayati implementation is a separate,
explicitly provisional kernel profile; it is executable but is not a claim of
regional authenticity. The normative theory for the current maqām module is
under `theory/`; the implementation is isolated under
`include/mq/kernel`, `src/kernel`, `apps/kernel`, and `tests/kernel`.

Pitch is calculated from symbolic ratio and contextual constraints. No scale
array, 24-TET table, neural network, gradient descent, or backpropagation is the
grammar authority.

## Build and run

```sh
make kernel
./build/kernel 2
make kernel-test
```

The optional unsigned seed drives the neutral executable generation fixture.
The provisional Bayati route is available as `./build/kernel bayati [seed]`.
Both print complete candidates and exact timed structural targets; neither is
an audio renderer or a regional-authenticity claim.

`make test` is an alias for the kernel law suite. Rejected legacy prototype
sources are not part of this checkpoint.

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
| `synthesis` | separate audio interface, instrument adapters, and acoustic validation |
| legacy directories | rejected generator/synthesizer retained for diagnosis |

## Scope

The kernel includes a provisional Egypt-Levant Bayati profile for executable
research. It is scoped by provenance, has explicit positive route rules, and
does not claim regional authenticity or corpus validation. Further regional
profiles require their own positive and negative rules, formulas, vocabulary,
pathways, and provenance evidence.

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
