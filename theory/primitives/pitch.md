# Pitch and tonal-function primitives

## Pitch is not a scale index

A performed pitch event contains at least:

\[
P=(\ell,\rho,\tau,\gamma,\kappa,\eta)
\]

where:

- \(\ell\): abstract pitch label relative to the current tonal frame;
- \(\rho\): tonal role;
- \(\tau\): active tonic hypothesis;
- \(\gamma(t)\): continuous pitch trajectory;
- \(\kappa\): contextual identity;
- \(\eta\): uncertainty and evidence provenance.

The same notated degree may have different pitch trajectories and functions in
different ajnas, directions, phrases, regions, and performers.

## Tonal roles

Roles are typed rather than inferred from integer degree alone:

- root tonic;
- local jins tonic;
- ghammaz or expected secondary tonic;
- emphasis/focal tone;
- pivot/common tone;
- approach tone;
- leading tone;
- suspension;
- cadence goal;
- baggage tone;
- passing color;
- neighbor;
- climax;
- registral boundary.

A tone can carry multiple roles with weights. A pivot can simultaneously be the
ghammaz of one jins and tonic of another.

## Relative identities

Pitch identity is stored relative to:

1. the global maqām root;
2. the active local tonic;
3. the register;
4. the current jins interpretation.

Absolute frequency is an exported performance parameter chosen after register
and performer context.

## Pitch trajectory

A tone is not represented by one cents value. Its trajectory is:

\[
\gamma(t)=\mu + b(t)+v(t)+o(t)+\epsilon(t)
\]

where:

- \(\mu\): contextual target center;
- \(b(t)\): approach and settling curve;
- \(v(t)\): intended pitch fluctuation;
- \(o(t)\): ornament trajectory;
- \(\epsilon(t)\): correlated performer variation.

The target center itself is a conditional distribution, not a universal ratio.

## Register

Register is relational:

- below-root baggage;
- root region;
- ghammaz region;
- upper-jins region;
- octave/jawāb region;
- extended upper region.

Each jins specifies where its identity is clearest and where it becomes
ambiguous or dependent on surrounding maqām context.

## Invariants

- notation classes such as “half-flat” are labels, not 24-TET instructions;
- octave equivalence is a hypothesis, not a global axiom;
- adjacent-jins relations can outweigh octave correspondence;
- no pitch is interpreted without a tonal and melodic context.
