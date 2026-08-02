# Music-seeder completion plan

This is the execution plan for the maqām and Faust portions of the project.
It preserves the full target: a sourced, executable Bayati slice that can
produce instrument-neutral performances and a Faust-backed C++ sound path,
while keeping regional theory and acoustic claims separate.

## Current checkpoint

The kernel portion is executable for a provisional Egypt-Levant Bayati profile:
four legal seeded routes, exact symbolic targets, timed events, and positive
law coverage. The synthesis boundary now contains a provisional C++ santur
model with three detuned courses, stiff-string partials, hammer-contact attack,
bridge/soundboard modes, and sympathetic tails. It renders a deterministic WAV.

The Faust side now has both a modal probe and a coupled-course source, plus a
dedicated generated-C++ runtime target that drives exact-contour frequency,
ornament, intensity, articulation, and strike controls through the shared WAV
boundary. Bayati now also carries a direction-conditioned exact timing policy
into both renderers; it is explicitly provisional execution data, not an
authenticity claim.
Bayati phrase/motif/ornament coverage and instrument measurements remain open.
Exact head `985464504bf3f732a9ab3cbb5ce3416fa4b6bffb` passed the remote workflow
in run `30755938899`, including the Faust probe and deterministic WAV artifact.

## Completion standard

The project is complete for this plan only when all of these are true:

1. A clean remote build validates the kernel, sanitizer paths, Faust code
   generation, and the synthesis adapter.
2. A seeded run produces a complete legal Bayati performance plan with exact
   structural pitch authority, contextual trajectories, timing, articulation,
   ornament intent, and explicit provenance.
3. The plan reaches a C++ adapter without MIDI, 12-TET, or a fixed cents array
   becoming musical authority.
4. The adapter produces a WAV render through a Faust-generated C++ model and
   rejects unsupported intents explicitly.
5. The hammered-dulcimer path accounts for coupled strings, hammer contact,
   bridge/soundboard behavior, dispersion, sympathetic energy, and overlap;
   a generic modal tone is not accepted as instrument completion.
6. Bayati claims are limited to the sourced region/repertoire/profile scope.
   Unknown rules remain rejected or explicitly provisional; they are never
   silently filled by a neutral or Western fallback.

Software tests prove implementation laws. They do not prove Bayati authenticity
or instrument plausibility. Those require the evidence and acoustic gates below.

## Execution order

### 0. Baseline, evidence, and remote capacity

Deliverables:

- clean-tree and exact-head report;
- Git-pinned audio sources;
- free remote debug, release, sanitizer, and Faust jobs;
- ledger entries that distinguish implemented, provisional, unverified, and
  rejected claims.

Evidence required:

- `make kernel-test`;
- `make kernel` and deterministic CLI seeds;
- remote GCC 14 and sanitizer results;
- Faust compiler generated-C++ probe from the pinned revisions.

Current state: local kernel, CLI, and independent synthesis evidence exists; the
Faust runtime path is implemented but the remote jobs have not yet run.

### 1. Universal kernel contract

Deliverables:

- finish strong typing behind the remaining generic `Identity` authorities;
- retain exact symbolic pitch expressions until the synthesis conversion
  boundary;
- add typed phrase, sayr, path, modulation, trajectory, timing, ornament,
  articulation, release, strand, and seed-partition state;
- derive complete plans only after all legal alternatives are complete;
- add laws for order independence, scope restoration, resource failure, and
  unsupported context.

Evidence required:

- each new type has a law test and an explicit failure path;
- no hard-coded maqām scale/cents arrays;
- no implicit generic-identity escape hatch;
- current neutral fixture remains a mechanism test only.

### 2. Evidence and federated maqām data

Deliverables:

- documentary theory records with source passage, location, claim class,
  competing interpretations, and provenance;
- a small reviewed Bayati performance casebook containing phrase boundaries,
  active jins, tonicization, cadence, motif/cell, timing, ornament, and
  disagreement fields;
- a separate calibration interface for later statistics, never an execution
  gate for the first provisional generator;
- rights and source manifests.

Evidence required:

- every regional rule names a source and claim class;
- attested, corroborated, profile-specific, analytical, provisional, and
  unknown claims remain distinguishable;
- no rule is promoted to “common” or “forbidden” without source or corpus
  evidence.

### 3. Sourced Bayati profile

Deliverables:

- an explicitly scoped Bayati profile, initially named and regional rather
  than universal;
- typed root jins and secondary jins descriptors with roles, registers,
  baggage, emphasis, entry/exit cells, cadence families, and gesture families;
- positive and negative modulation/tonicization graph edges;
- establishment, internal development, climax, return, and closure obligations;
- complete sayr routes with dependency ordering and retained proofs;
- phrase and motif productions with identity-preserving variation;
- contextual intonation formulas and trajectory variants, not one scale table;
- timing and articulation formulas with declared provisional status where data
  is insufficient.

Evidence required:

- Bayati `make_bayati()` returns a complete scaffold;
- every route reaches a legal root return and cadence;
- unsupported direct edges fail with named diagnostics;
- the profile test harness covers all eight Bayati case requirements and the
  minimum intonation comparisons in `theory/case/bayati.md`;
- no regional authenticity claim is made before practitioner/corpus gates.

### 4. Bayati generation

Deliverables:

- Bayati grammar compiles to complete candidates;
- deterministic seeds select only among complete legal candidates;
- generated plans expose structural event, phrase, sayr, path, and provenance
  traces;
- failures identify missing obligations, unsupported paths, or insufficient
  evidence without mutating state.

Evidence required:

- multiple seeds produce reproducible but non-identical legal alternatives;
- illegal candidates remain rejected under profile narrowing;
- root establishment, internal color, return, cadence, and question/response
  are visible in generated traces;
- Bayati output is distinguishable from the neutral fixture by executable
  profile identity and consequences.

### 5. Complete performance boundary

Deliverables:

- phrase-conditioned score and performed time maps;
- duration, pause, release, dynamics, articulation, ornament, and strand
  intents;
- continuous pitch trajectory authority with exact control points and explicit
  numeric conversion boundary;
- adapter coverage matrix and refusal trace;
- independent synthesis seed partition.

Evidence required:

- exact plan laws cover timing conservation, positive durations, release order,
  strand dependencies, and ornament host preservation;
- trajectory continuity is tested across onset, stable, release, attraction,
  and vibrato regions;
- no adapter test converts through MIDI note identity or a static Western scale.

### 6. Faust-generated C++ adapter and first audio

Deliverables:

- Faust physical-model sources remain pinned by Git;
- generated C++ is built outside `mq/kernel` and consumed through a narrow
  synthesis adapter;
- the adapter maps continuous frequency, intensity, articulation, strike, and
  release intent;
- a deterministic block scheduler renders at least one complete Bayati plan;
- a WAV writer produces an auditable artifact with sample rate and conversion
  metadata;
- unsupported gesture/trajectory combinations return explicit refusal data.

Evidence required:

- clean remote Faust generation and C++ compilation;
- deterministic render hash for a fixed seed/model/configuration;
- non-silent WAV with bounded sample values and event-aligned attacks;
- frequency trajectory inspection at the declared conversion boundary;
- no Faust runtime or source dependency enters `mq/kernel`.

### 7. Hammered-dulcimer/santur physical model

Deliverables:

- course and paired-string state with detuning;
- stiff-string dispersion;
- nonlinear hammer/string contact;
- bridge mobility and soundboard modal/radiation state;
- sympathetic coupling, damping, muting, and restrike behavior;
- strike-position and force mapping from instrument-neutral intent;
- geometry/material parameter manifest and model versioning.

Evidence required:

- isolated course attacks and decays;
- repeated strikes and adjacent-course overlap;
- fast-cell masking and transient-energy measurements;
- pitch/dynamic-dependent decay and beating;
- rejection when a narrow resonance dominates or later attacks disappear;
- reference recordings/measurements and declared tolerances.

### 8. Release and audit

Deliverables:

- clean-tree release commit(s) with deliberate scope;
- exact remote commit and workflow evidence;
- updated `STATUS.md`, theory ledger, synthesis validation, and research debt;
- artifact manifest for generated C++, WAVs, configs, and source revisions;
- explicit list of claims still provisional or unverified.

The objective is not complete merely because the neutral kernel, a generic
modal probe, or a successful compilation exists. The Bayati vertical slice and
the Faust-backed audible path must be exercised together.
