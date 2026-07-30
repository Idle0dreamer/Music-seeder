# Contextual intonation

No fixed cents array, equal-tempered quarter-tone grid, or universal set of
ratios defines performance intonation.

## Pitch target

For a structural event \(u\), the target and trajectory are solved from:

\[
\operatorname{Intone}\!\left(u\mid
J,M,\rho,d,n_-,n_+,k,g,p,r,\iota\right)
\]

where:

- \(J\): jins;
- \(M\): parent maqam and tonal level;
- \(\rho\): structural pitch role;
- \(d\): melodic direction and turn behavior;
- \(n_-,n_+\): neighboring structural roles;
- \(k\): motif and cadence context;
- \(g\): region and repertoire profile;
- \(p\): performer;
- \(r\): register;
- \(\iota\): intonational intent, including expressive attraction.

The output \(c_u(t)\) is a constrained cents trajectory, not one scalar.

## Composed formula

A profile supplies explicit, provenance-bearing terms:

\[
c_u(t)=
\mu_{\rho,J,M}(t)
+\delta_{\text{direction}}(t)
+\delta_{\text{neighbors}}(t)
+\delta_{\text{cadence}}(t)
+\delta_{\text{region}}(t)
+\delta_{\text{performer}}(t)
+\epsilon_s(t),
\]

where \(\epsilon_s\) is an optional bounded seeded deviation. Competing
intonational practices remain named formula variants rather than being averaged
or collapsed into a learned distribution.

The model retains:

- onset, stable region, and release;
- direction-dependent attraction;
- phrase and cadence position;
- local uncertainty;
- correlations among tones within one gesture.

## Pitch derivation

Scale-like summaries may be calculated after the contextual solver:

\[
\hat c_{\rho}=c_u(t_\text{stable}\mid\text{fully specified context}).
\]

Such a summary is a query result, never the generative foundation.

## Constraints

1. Octave equivalence is a configurable repertoire and analysis assumption,
   not blindly forced on every measured trajectory.
2. Pitch order and collision constraints are enforced after expressive
   deviations.
3. Neutral intervals remain contextual fields or named ranges, not labels
   mapped to exactly 150 or 350 cents.
4. Sparse contexts return a declared range or “unsupported,” not an averaged
   default.
5. Performer adaptation changes correlated tendencies, not isolated pitches.

## Estimation

Pitch tracks require manual correction around attacks, ornaments, beating, and
overlapping sources. Stable-tone extraction alone is insufficient. Measurements
must link trajectories to melodic roles and phrase context before constants are
reviewed into a profile.
