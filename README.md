# MaqamForge

MaqamForge is being rebuilt around a non-ML, mathematically explicit operator
algebra for maqam performance. The shared kernel defines typed musical
operators and exact composition laws; regional, school, repertoire, performer,
and instrument variants are reconstructed from declarative, provenance-bearing
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

`make test` still verifies that the isolated legacy prototype compiles, but its
passing tests do not establish musical validity.

## Source map

| Area | Responsibility |
| --- | --- |
| `theory/algebra` | sorts, operators, semantics, laws, regional reconstruction |
| `theory/pitch` | exact calculated pitch-field specification |
| `include/mq/kernel` | public C++23 kernel types |
| `src/kernel` | exact arithmetic, solver, profiles, operators, evaluator |
| `tests/kernel` | executable algebra and reconstruction laws |
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
- Mayer Alexander and Herbert Rust,
  [“A method for an acoustical comparison of the Hammered Dulcimer”](https://pub.dega-akustik.de/DAGA_2010/data/articles/000215.pdf),
  DAGA 2010, for hammer-position variability, strong
  string/bridge/soundboard coupling, and decay behavior.
- V. Välimäki et al.,
  [“Sound Synthesis of the Harpsichord Using a Computationally Efficient Physical Model”](https://www.ee.columbia.edu/~dpwe/e6820/papers/ValPK04-harpsi.pdf),
  2004, for the useful struck/plucked
  string decomposition into excitation, string loss/dispersion, and a shared
  soundboard response.

See `theory/README.md` and `theory/implementation/kernel.md` for the model and
the exact implementation boundary.
