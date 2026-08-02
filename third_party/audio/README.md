# Audio source checkouts

These are Git-pinned source inputs for the future synthesis subsystem. They
are not kernel dependencies, and they are not linked into the current build.
The purpose of recording them now is to make the physical-sound work
reproducible without copying large libraries into project-owned source trees.

## Pinned revisions

| Checkout | Revision | Role |
| --- | --- | --- |
| `third_party/audio/faust` | Faust `2.85.9`, commit `3c44e5cb64be06f8ac7c025f1633178276f35d8a` | Build-time Faust compiler and physical-modeling tools, including `mesh2faust` |
| `third_party/audio/faustlibraries` | `master` at `2598378dce8b335d1a53a4b9116422c3a91060e7` | Faust physical-modeling library definitions |
| `third_party/audio/stk` | STK `5.0.1`, commit `1cb2c81d04922b6337073ecb44d83bf2208690f5` | Permissively licensed C++ reference models and DSP classes |
| `third_party/audio/q` | `master` at `8942947c9d7a644035579d01aecf2a1046c2d896` | C++ DSP support reference |
| `third_party/audio/tuning-library` | `release_1.1.0` at `64a939111891cb698f37d6ca9ecfc4b8c260053d` | Optional `.scl`/`.kbm` interoperability |

The revisions were resolved from the upstream Git repositories on 2026-08-02.
Do not replace a commit with a branch name in a build or validation claim.

## Retrieval

On a capable or hosted machine, initialize the checkouts with shallow filtered
Git history:

```sh
git submodule update --init --filter=blob:none --jobs=1
```

The constrained development host should fetch one checkout at a time and
should not build Faust, `mesh2faust`, or an instrument adapter locally. The
online build workflow is the intended place for compiler and model validation.
The `faust-probe` job in `.github/workflows/kernel.yml` builds the pinned Faust
compiler and generates C++ from both `synthesis/models/modal_probe.dsp` and
`synthesis/models/santur_courses.dsp`; the synthesis job also builds and runs
the generated-Faust adapter.

## Ownership and licensing

- Faust is a build-time model specification and code-generation input. Its
  generated C++ must cross into `synthesis/` through an explicit adapter.
- STK, Q, and Surge Tuning remain independently licensed external works. Their
  license files and any included audio assets must be reviewed before linking.
- RipplerX and Resonarium remain research references only because their GPL
  terms are incompatible with treating them as unexamined project
  dependencies.
- Nothing under these checkouts may become an authority for maqam grammar,
  regional evidence, or kernel legality.
