# Music-seeder guidance

## Authority

- Read `STATUS.md`, `theory/README.md`, and
  `theory/implementation/kernel.md` before changing the kernel.
- Treat `theory/` as normative and update its implementation ledger only when
  executable evidence supports a claim.
- Keep rejected generator or synthesizer ideas outside the kernel.

## Architecture

- Use C++23. Do not use Python.
- Do not use machine learning, backpropagation, or opaque learned grammar.
- Do not encode maqamat or ajnas as hard-coded scale or cents arrays.
- Keep exact rational and symbolic pitch expressions until the exported
  performance plan reaches its declared numerical boundary.
- Keep audio synthesis outside `theory/` and `mq/kernel`; synthesis adapters
  consume instrument-neutral kernel output through a one-way interface.
- Keep operator semantics universal and reconstruct traditions through explicit,
  provenance-bearing profile operations.
- Missing regional knowledge has no generic fallback. Use neutral fixtures until
  sourced rules exist.
- Random seeds select among complete legal alternatives and never alter legality.

## Structure

- Keep files short, modular, and named by one responsibility.
- Prefer directories over compound file names.
- Separate algebra, profile reconstruction, pitch, grammar, and performance.
- Keep synthesis, instrument adapters, and acoustic validation under
  `synthesis/`, independent of musical-language validation.
- Treat optimistic claims as research debt: state the missing evidence and build
  the smallest complete seam that removes it.

## Validation

Run the applicable subset:

```sh
make kernel-test
make kernel-sanitize
make kernel
./build/kernel
```

AddressSanitizer remains a separate environmental check. Do not count unavailable
legacy prototype files as kernel validation.
