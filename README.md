# MaqamForge

MaqamForge is being rebuilt around a non-ML, mathematically explicit operator
algebra for maqam performance. The shared kernel defines typed musical
operators and exact composition laws; regional, school, repertoire, performer,
and form variants are reconstructed from declarative, provenance-bearing
profile patches.

The old seed-driven Bayati generator and synthesized audio remain as rejected
diagnostic prototypes. They are not the project authority. The normative theory
is under `theory/`; the new C++23 implementation is isolated under
`include/mq/kernel`, `src/kernel`, `apps/kernel`, and `tests/kernel`.

Pitch is calculated from symbolic ratio and contextual constraints. No scale
array, 24-TET table, neural network, gradient descent, or backpropagation is the
grammar authority.

## Build and run

```sh
make kernel
./build/kernel
make kernel-test
```

`make test` is an alias for the kernel law suite. Rejected legacy prototype
sources are not part of this checkpoint.

## Source map

| Area | Responsibility |
| --- | --- |
| `theory/algebra` | sorts, operators, semantics, laws, regional reconstruction |
| `theory/pitch` | exact calculated pitch-field specification |
| `include/mq/kernel/grammar` | finite terms, guards, scope policy, derivations |
| `include/mq/kernel/state` | decomposed musical-state projections |
| `include/mq/kernel/path` | constrained modulation graph rules |
| `include/mq/kernel/pitch` | symbolic expressions and equality system |
| `include/mq/kernel/profile` | rules, patches, and effective profiles |
| `src/kernel/*` | implementations separated by the same domains |
| `tests/kernel` | executable algebra and reconstruction laws |
| `synthesis` | separate audio interface, instrument adapters, and acoustic validation |
| legacy directories | rejected generator/synthesizer retained for diagnosis |

## Scope

The kernel currently uses neutral reconstruction fixtures. It does not claim an
Egyptian, Levantine, or other regional grammar. A real profile will be admitted
only when its positive and negative rules, formulas, vocabulary, pathways, and
provenance satisfy the theory gates.

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
